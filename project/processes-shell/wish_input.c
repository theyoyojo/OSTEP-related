#include "wish_input.h"

#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

bool insert_spaces_where_needed(struct wish_input * new_input) {
	bool success_code ;
	ssize_t char_counter, new_size ;
	char * modified_line ;

	/* three times size increase is theoretical max */
	if (!new_input || !(modified_line = calloc(sizeof(char),new_input->line_size * 3))) return false ;
	success_code = true ;

	new_size = 0 ;
	for (char_counter = 0; char_counter < new_input->line_size; ++char_counter) {
		if (new_input->line[char_counter] == '>' || new_input->line[char_counter] == '&') {
			modified_line[new_size++] = ' ' ;
		}
		modified_line[new_size++] = new_input->line[char_counter] ;	
		if (new_input->line[char_counter] == '>' || new_input->line[char_counter] == '&') {
			modified_line[new_size++] = ' ' ;
		}
	}

	free(new_input->line) ; new_input->line = modified_line ;
	new_input->line_size = new_size ;

	return true ;
}

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

	/* +1 for NULL terminating the vector */
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

struct wish_input * wish_input_new(FILE * source_file) {
	struct wish_input * new_input ;
	char * new_line ;
	ssize_t new_line_size ;

	new_input = NULL ;
	new_line = NULL ;
	new_line_size = 0 ;

	if (!(new_input = (struct wish_input *)calloc(sizeof(struct wish_input), 1))) {
		goto the_end ;
	}

	/* case: no input file or cannot get input from file (i.e nothing to work with) */
	if (!source_file || (new_line_size = getline(&new_line, (size_t *)&new_line_size, source_file)) < 0) {
		pDEBUG("no input...") ;
		if (new_line) {
			free(new_line) ; new_line = NULL ;
		}
		new_input = NULL ;
	}
	/* case: input detected */
	else {
		new_input->line = new_line ;
		new_input->line_size = new_line_size ;
		if (strstr(new_input->line, ">") || strstr(new_input->line, "&")) {
			insert_spaces_where_needed(new_input) ;
		}
		
		// FIXME: is there a memory leak when tokenize_input() fails?
		if (!tokenize_input(new_input)) {
		/* case: unable to tokenize */
			free(new_input) ;
			new_input = NULL ;
		}

	}
		/* case: unsuccessful allocation */
		/* new_input is already NULL, this will be returned */
the_end:
	return new_input ;
}

struct wish_input * wish_input_new_interactive(FILE * input_file) {
	write(STDOUT_FILENO, WISH_INPUT_PROMPT, strlen(WISH_INPUT_PROMPT)) ;
	return wish_input_new(input_file) ;	
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

void wish_input_delete(struct wish_input * input) {
	size_t counter ;

	if (input) {
		for (counter = 0; counter < input->size; ++counter) {
			free(input->tokens[counter]) ;
		}
		if (input->line) free(input->line) ;
		free(input->tokens) ;
		free(input) ;
	}
	else {
		pDEBUG("invalid object, not freeing") ;
	}
}

struct wish_input * wish_input_split_by_delim(struct wish_input * input, char delim) {
	size_t token_counter ;
	struct wish_input * new_input = NULL ;
	for (token_counter = 0; token_counter < input->size; ++token_counter) {
		//pDEBUG("starting wish_input_split_by_delim for loop") ;
		if (!input->tokens[token_counter]) {
			pDEBUG("reached null token, giving up") ;
			break ;
		}
		else
		if (*input->tokens[token_counter] == delim) {
			if (!(new_input = (struct wish_input *)calloc(sizeof(struct wish_input), 1))) {
				pDEBUG("BAD ALLOC") ;
				break ;
			}
			else {
				//pDEBUG("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX") ;
				*new_input = WISH_INPUT_DEFAULT ;
				pDEBUG("set contents of new input to defaults") ;
				new_input->capacity = input->size - token_counter ; /* + 1 for null terminator */
				if (!(new_input->tokens = (char **)malloc(sizeof(char *) * new_input->capacity))) {
					pDEBUG("BAD ALLOC part 2") ;
					free(new_input) ; new_input = NULL ;
					break ;
				}
				//new_input->size = new_input->capacity - 1 ;/* size does not include null term */
				pDEBUG("remove the &, free it, and replace with NULL") ;
				free(input->tokens[token_counter]) ;
				input->tokens[token_counter] = NULL ;
				while (new_input->size < new_input->capacity - 1) {
				/* new input takes responsibility for its dynamic string data */
					new_input->tokens[new_input->size++] = strdup(input->tokens[++token_counter]) ;
					free(input->tokens[token_counter]) ;
				}
				new_input->tokens[new_input->size] = NULL ;
				input->size -= (new_input->size + 1) ; /* + 1 for & that became & */
				break ;
			}
		pDEBUG("probably should not get here") ;
		break ;
		}
	}
	
	return new_input ;
}
