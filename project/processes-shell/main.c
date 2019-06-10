#undef DEBUG
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "wish_parse.h"
#include "wish_error.h"

int main(int argc, char * argv[]) {
	FILE * script_file ;

	struct wish_context * context = wish_context_new() ;
	if (!context) {
		pDEBUG("unable to alloc new context") ;
		error() ;
		/* gotos > multiple returns in a function */
		goto the_end ;
	}

	script_file = NULL ;
	if (argc > 2) {
	/* case: invalid usage */
		pDEBUG("bad usage bro") ;
		error() ;
	}
	else
	if (argc > 1) {
	/* case: batch usage */
		if ((script_file = fopen(argv[1], "r"))) {
			context->mode = WISH_MODE_BATCH ;
			context->input_fd = fileno(script_file) ;
		}
		else {
			pDEBUG("unable to open script file") ;
			error() ;
		}
	}
	else {
	/* case: interactive usage */
		context->mode = WISH_MODE_INTERACTIVE ;
	}

	while (wish_context_is_active(context)) {
		if (!wish_context_get_input(context)) {
			context->mode = WISH_MODE_INACTIVE ;
			continue ;
		}
#ifdef DEBUG
		wish_input_print_stdout(context->input) ;
#endif
		if (wish_parse(context) < 0) {
			pDEBUG("error parsing input") ;
			error() ;
		}
	}

	wish_context_delete(&context) ;
	if (script_file) {
		fclose(script_file) ;
	}

the_end:
	return 0 ;
}
