#include "wish_input.h"

#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/* private tokenization function */
static bool tokenize_input(struct wish_input * dest, char * line, ssize_t line_size) {
	// TODO: stub
	dest->tokens = (char **)malloc(sizeof(char*)) ;
	dest->tokens[0] = strndup(line, line_size) ;
	dest->size = 1 ;

	return true ;
}

struct wish_input * wish_input_new(int input_fd) {
	struct wish_input * new_input ;
	char * new_line ;
	ssize_t new_line_size ;
	FILE * source_file ;

	new_input = NULL ;
	new_line = NULL ;
	new_line_size = 0 ;

	source_file = fdopen(input_fd, "r") ;

	/* case: no input */
	if (!source_file || (new_line_size = getline(&new_line, &new_line_size, source_file)) < 0) {
		printf("no input (nls=%zd)\n", new_line_size) ;
		new_input = NULL ;
	}
	/* case: input detected */
	else {
		new_input = (struct wish_input *)calloc(sizeof(struct wish_input), 1) ;
		/* case: successful allocation */
		if (new_input) {
			if (!tokenize_input(new_input, new_line, new_line_size)) {
			/* case: unable to tokenize */
				free(new_input) ;
				new_input = NULL ;
			}
			// TODO history

		}
		/* case: unsuccessful allocation */
		/* new_input is already NULL, this will be returned */
	}

	//fclose(source_file) ;

	return new_input ;
}

struct wish_input * wish_input_new_interactive(void) {
	write(STDOUT_FILENO, WISH_INPUT_PROMPT, strlen(WISH_INPUT_PROMPT)) ;
	return wish_input_new(STDIN_FILENO) ;	
}

bool wish_input_print_stdout(struct wish_input * input) {
	if (!input) return false ;

	static char tok_open[] = "[" ;
	static char tok_close[] = "]\n" ;
	static char tok_delim[] = ", " ;
	static char tok_empty[] = "" ;

	size_t counter ;
	write(STDOUT_FILENO, tok_open, strlen(tok_open)) ;
	for (counter = 0; counter < input->size; ++counter) {
		printf("\"%s\"%s", input->tokens[counter],
			counter != input->size - 1 ? tok_delim : tok_empty) ;

	}
	write(STDOUT_FILENO, tok_close, strlen(tok_close)) ;

	return true ;
}

void wish_input_delete(struct wish_input ** input) {
	size_t counter ;

	if (input && *input) {
		for (counter = 0; counter < (*input)->size; ++counter) {
			free((*input)->tokens[counter]) ;
		}
		free(*input);*input = NULL ;
	}
	else {
		fprintf(stderr, "wish_input_delete(): invalid object\n") ;
	}
}
