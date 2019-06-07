#include "wish_context.h"
#include <stdlib.h>

void wish_input_sanitize(void) ;

struct wish_context * wish_context_new(enum wish_mode mode) {
	struct wish_context * new_context ;

	new_context = NULL ;

	if (!(new_context = (struct wish_context *)calloc(sizeof(struct wish_context), 1))) {
		pDEBUG("malloc error") ;
		return NULL ;
	}

	*new_context = WISH_CONTEXT_DEFAULT ;

	new_context->mode = mode ;

	return new_context ;
}

void wish_context_delete(struct wish_context ** context_ptr) {
	if (context_ptr) {
		struct wish_context * context = *context_ptr ;
		if (context) {
			wish_input_delete(&context->input) ;
			free (context);*context_ptr = NULL ;
		}
		else {
			pDEBUG("invalid object passed to function") ;
		}
	}
	else {
		pDEBUG("invalid pointer passed to function") ;
	}
}

void wish_input_sanitize(void) {
	// FIXME what is this garbage
	return ;
}

bool wish_context_get_input(struct wish_context * context) {
	struct wish_input * (*get_input)(int input_fd) ;

	if (context->input) wish_input_delete(&context->input) ;

	switch (context->mode) {
		case WISH_MODE_INTERACTIVE:
			get_input = wish_input_new_interactive ;
			break ;
		case WISH_MODE_BATCH:
		default:
			get_input = wish_input_new ;
			break ;
	}

	return !!(context->input = get_input(context->input_fd)) ;
}

/* executed when no builtins are available */
bool wish_context_parse_input(struct wish_context * context) {
	bool success_code = true ;
	
	pDEBUG("STUB") ;
	
	return true ;
}


char * wish_context_get_token_at(struct wish_context * context, size_t index) {
	if (index > context->input->size) {
		return NULL ;
	}

	return context->input->tokens[index] ;
}
