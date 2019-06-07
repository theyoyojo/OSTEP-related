#ifndef WISH_INPUT_H
#define WISH_INPUT_H "wish_input.h"

#include <stdbool.h>
#include <stdio.h>

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

#define WISH_INPUT_PROMPT "wish> "

struct wish_input * wish_input_new(int input_fd) ;
struct wish_input * wish_input_new_interactive(int input_fd) ;

bool wish_input_print_stdout(struct wish_input * input) ;

void wish_input_delete(struct wish_input ** input) ;

#endif /* #ifndef "wish_input.h" */
