#include "wish_input.h"

#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

/* that string better be null-terminated */
/* returns number of characters removed */
size_t truncate_trailing_whitespace(char * string) {
	size_t 	characters_removed, string_length ;
	int 	counter ;

	string_length = strlen(string) ;

	characters_removed = 0 ;
	for (counter = string_length - 1; counter > 0; --counter) {
		if (isspace(string[counter])) {
			string[counter] = '\0' ;
			++characters_removed ;
		}
		else {
			break ;
		}
	}

	return characters_removed ;
}

/* private tokenization function but it is very much a hack */
static bool tokenize_input(struct wish_input * dest) {
	char * tok, ** temp ;
	bool success_code = true ;

	/* assume (without evidence) that each line of input has like four tokens on average */
	size_t baseless_size_assumption = 4 ;

	dest->capacity = baseless_size_assumption ;

	/* + 1 for NULL terminating the vector */
	dest->tokens = (char **)malloc(sizeof(char*) * (dest->capacity + 1)) ;

	tok = NULL ;
	temp = NULL ;

	tok = strtok(dest->line, " \t\n") ;
	dest->size = 0 ;
	if (tok) do {
		if (dest->size + 1 > dest->capacity) {
			temp = (char **)realloc(dest->tokens, sizeof (char *) * (dest->capacity *= 2 + 1) ) ;
			if (!temp) {
				pDEBUG("realloc error") ;
				dest->capacity /= 2 ;
				success_code = false ;
				break ;
			}
			dest->tokens = temp ;
		}
		/* printf("got tok: %s (size: %zd)\n", tok, strlen(tok)) ; */
		if (!isspace(*tok)) {
			dest->tokens[dest->size++] = strdup(tok) ;
		}

	} while ((tok = strtok(NULL, " \t\n"))) ;

	/* if one or more tokens exist, remove any whitespace at end of last token */
	if (dest->size > 0) {
		truncate_trailing_whitespace(dest->tokens[dest->size - 1]) ;
	}
	

	/* to use this as argv, the last element in the array must be NULL */
	dest->tokens[dest->size] = NULL ;

	return success_code ;
}

struct wish_input * wish_input_new(int input_fd) {
	struct wish_input * new_input ;
	char * new_line ;
	ssize_t new_line_size ;
	FILE * source_file ;
	int input_fd_copy ;

	new_input = NULL ;
	new_line = NULL ;
	new_line_size = 0 ;
	
	/* work with a copy of input_fd to avoid closing it when fclose is called */
	input_fd_copy = dup(input_fd) ;

	if (input_fd_copy < 0) {
		pDEBUG("cannot duplicate input_fd") ;
		return NULL ;
	}

	source_file = fdopen(input_fd_copy, "r") ;

	/* case: no input */
	if (!source_file || (new_line_size = getline(&new_line, (size_t *)&new_line_size, source_file)) < 0) {
		pDEBUG("no input...") ;
		if (new_line) {
			free(new_line) ; new_line = NULL ;
		}
		new_input = NULL ;
	}
	/* case: input detected */
	else {
		new_input = (struct wish_input *)calloc(sizeof(struct wish_input), 1) ;
		/* case: successful allocation */
		if (new_input) {
			new_input->line = new_line ;
			new_input->line_size = new_line_size ;
			if (!tokenize_input(new_input)) {
			/* case: unable to tokenize */
				free(new_input) ;
				new_input = NULL ;
			}
			// TODO history

		}
		/* case: unsuccessful allocation */
		/* new_input is already NULL, this will be returned */
	}

	fclose(source_file) ;

	return new_input ;
}

struct wish_input * wish_input_new_interactive(int input_fd) {
	write(input_fd, WISH_INPUT_PROMPT, strlen(WISH_INPUT_PROMPT)) ;
	return wish_input_new(input_fd) ;	
}

bool wish_input_print_stdout(struct wish_input * input) {
	if (!input) return false ;

	static char tok_open[] = "[" ;
	static char tok_close[] = "]\n" ;
	static char tok_delim[] = ", " ;
	static char tok_null[] = "(null)" ;

	static char str_newline[] = "\\n" ;
	static char str_null[] = "\\0" ;
	static char str_tab[] = "\\t" ;
	char current = '\0' ;


	/* char buff[16] ; */
	/* memset(buff, 0, 16) ; */

	size_t token_counter, character_counter ;
	write(STDOUT_FILENO, tok_open, strlen(tok_open)) ;
	/* input->size + 1 to display the "null terminator" element in the argv input */
	for (token_counter = 0; token_counter < input->size + 1; ++token_counter) {
		input->tokens[token_counter] ? ({ /* case: token exists (last must be null) */
		write(STDOUT_FILENO, "\"", 1) ;
		for (character_counter = 0; character_counter < strlen(input->tokens[token_counter]); ++character_counter) {
			switch (current = input->tokens[token_counter][character_counter]) {
				case '\n':
					write(STDOUT_FILENO, str_newline, strlen(str_newline)) ;
					break ;
				case '\t':
					write(STDOUT_FILENO, str_tab, strlen(str_tab)) ;
					break ;
				case '\0':
					write(STDOUT_FILENO, str_null, strlen(str_null)) ;
					break ;
				default:
					write(STDOUT_FILENO, &current, 1) ;
			} /* switch on current character */
		} /* for character in token */


		write(STDOUT_FILENO, "\"", 1) ;
		/* print a delimiter after all tokens except the last null guy */
		if (token_counter != input->size) {
			write(STDOUT_FILENO, tok_delim, strlen(tok_delim)) ;
		}
		}) : /* FALLBACK case: token does not exist */
		write(STDERR_FILENO, tok_null, strlen(tok_null)) ;
		/* end FALLBACK case */

	//	printf("(%zu) ", strlen(input->tokens[token_counter])) ;

	} /* for token in wish_input */
	write(STDOUT_FILENO, tok_close, strlen(tok_close)) ;

	return true ;
}

void wish_input_delete(struct wish_input ** input) {
	size_t counter ;

	if (input && *input) {
		for (counter = 0; counter < (*input)->size; ++counter) {
			free((*input)->tokens[counter]) ;
		}
		free((*input)->line) ;
		free((*input)->tokens) ;
		free(*input);*input = NULL ;
	}
	else {
		pDEBUG("invalid object, not freeing") ;
	}
}
