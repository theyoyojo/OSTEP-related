#include <stdio.h>
#include <string.h>

#include "wish_context.h"

void error(void) ;

int main(int argc, char * argv[]) {
	/* setup */

	enum wish_mode mode ;

	if (argc > 1) {
		mode = WISH_MODE_BATCH ;
	}
	else {
		mode = WISH_MODE_INTERACTIVE ;
	}

	struct wish_context * context = wish_context_new(mode) ;

	while (wish_context_is_active(context)) {
		if (!wish_context_get_input(context)) {
			error() ;
			continue ;
		}

		if (!wish_context_parse_input(context)) {
			error() ;
		}

		wish_input_print_stdout(context->input) ;
	}

	return 0 ;
}

void error(void) {
	static char error_message[30] = "An error has occurred\n" ;
	write(STDERR_FILENO, error_message, strlen(error_message)) ;
}
