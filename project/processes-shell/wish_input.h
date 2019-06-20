#ifndef WISH_INPUT_H
#define WISH_INPUT_H "wish_input.h"

#include <stdbool.h>
#include <stdio.h>

//#define DEBUG

#ifdef DEBUG
#define pDEBUG(msg) fprintf(stderr, "%s:%d %s(): " msg "\n", __FILE__, __LINE__,  __func__)
#else
#define pDEBUG(msg) (void)msg
#endif /* #ifndef DEBUG */


struct wish_input {
	char 			* line ;
	ssize_t 		line_size ;
	char 			** tokens ;
	size_t 			size ;
	size_t 			capacity ;
} ;

#define WISH_INPUT_DEFAULT (struct wish_input) { \
	.line = 		NULL, \
	.line_size = 		0, \
	.tokens = 		NULL, \
	.size = 		0, \
	.capacity = 		0, \
}

#define WISH_INPUT_PROMPT "wish> "

struct wish_input * wish_input_new(FILE * input_file) ;
struct wish_input * wish_input_new_interactive(FILE * input_file) ;

struct wish_input * wish_input_split_by_delim(struct wish_input * input, char delim) ;

bool wish_input_print_stdout(struct wish_input * input) ;

void wish_input_delete(struct wish_input * input) ;

#endif /* #ifndef "wish_input.h" */
