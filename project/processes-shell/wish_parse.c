#include "wish_parse.h"
#include "wish_error.h"

#include <sys/wait.h>

int wish_parse_input(struct wish_context * context, size_t index) {
	int ret ;
	char * first_token ;

	// TODO scan for special actions

	/* what kind of nonsense does the user want the computer to attempt */
	first_token = wish_context_get_token_at(context, index, 0) ;

	/* case: no idea, just ignore it honestly it's probably blank line or something */
	if (!first_token) {
		pDEBUG("nooo first token, ignoring as null") ;
		ret = 0 ;
	}
	else
	/* case: if there is a builtin available for that command, run it */
	if (wish_builtin_available(context, index)) {
		ret = wish_builtin_exec(context, index,
			wish_builtin_string_to_enum(first_token)) ;
		if (ret < 0) {
			error() ;
		}
	}
	/* case: try and find an executable in the specified paths */
	else {
		/* bool code to int code (nonzero ret on success -> zero ret on success) */
		ret = !wish_context_parse_input(context, index) ;
	}

	return -ret ;
}

/* easter egg */
void _(){}

int wish_parse(struct wish_context * context) {
	int ret ;
	size_t input_counter ;
	struct wish_input * temp ;

	ret = 0 ;
	(void)context ;
	/* parallel commands bro */
//	printf("size before &split: %zu\n", context->size) ;
	while ((temp = wish_input_split_by_delim(context->input[context->size - 1], '&'))) {
	//	printf("got new thing: %p\n", temp) ;
		context->input[context->size++] = temp ;	
	}
	pDEBUG("hmm looks like temp is null time to move on") ;
	//printf("size __AFTER__ &split: %zu\n", context->size) ;

	pDEBUG("wish parse debug") ;
	///_();_();_();_();_();

	//printf("size == %zu\n", context->size) ;
	for (input_counter = 0; input_counter < context->size; ++input_counter) {
#ifdef DEBUG
		wish_input_print_stdout(context->input[input_counter]) ;
#endif /* DEBUG */
		ret -= wish_parse_input(context, input_counter) ;
	}
//	printf("ret == %d\n", ret) ;

	/* wait for all children to exit */
	while (context->children > 0) {
		wait(NULL) ;
		--context->children ;
	}

	return -ret ;
}
