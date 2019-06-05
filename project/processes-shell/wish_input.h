#ifndef WISH_INPUT_H
#define WISH_INPUT_H "wish_input.h"

#include <stdbool.h>
#include <stdio.h>

struct wish_input {
	char 			** tokens ;
	size_t 			size ;
} ;

#define WISH_INPUT_PROMPT "wish> "

struct wish_input * wish_input_new(int input_fd) ;
struct wish_input * wish_input_new_interactive(void) ;

bool wish_input_print_stdout(struct wish_input * input) ;

void wish_input_delete(struct wish_input ** input) ;

#endif /* #ifndef "wish_input.h" */
