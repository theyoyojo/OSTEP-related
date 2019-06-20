#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "wish_parse.h"
#include "wish_error.h"

int main(int argc, char * argv[]) {
	int script_file_fd = -1 ;
	int ret = 0 ;

	struct wish_context * context = wish_context_new() ;
	if (!context) {
		pDEBUG("unable to alloc new context") ;
		error() ;
		/* gotos > multiple returns in a function */
		goto the_end ;
	}

	script_file_fd = -1 ;
	if (argc > 2) {
	/* case: invalid usage: clean up and gtfo */
		pDEBUG("bad usage bro") ;
		wish_context_delete(&context) ;
		ret = 1 ;
		error() ;
		goto the_end ;
	}
	else
	if (argc > 1) {
	/* case: batch usage */
		if (!((script_file_fd = open(argv[1], 0)) < 0)) {
			context->mode = WISH_MODE_BATCH ;
			context->input_fd = script_file_fd ;
		}
		else {
			pDEBUG("unable to open script file") ;
			ret = 1 ;
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
		wish_input_print_stdout(context->input[0]) ;
#endif
		if (wish_parse(context) < 0) {
			pDEBUG("error parsing input") ;
			error() ;
		}
	}

	wish_context_delete(&context) ;
	if (script_file_fd > 0) {
		close(script_file_fd) ;
	}

the_end:
	pDEBUG("the_end") ;
	return ret ;
}
