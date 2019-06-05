#include "wish_context.h"
#include <stdlib.h>

#undef DEBUG

#ifdef DEBUG
#define pDEBUG(msg) puts(__func__ "(): " msg "\n")
#else
#define pDEBUG(msg) (void)msg
#endif /* #ifndef DEBUG */


void wish_input_sanitize(void) ;

struct wish_context * wish_context_new(enum wish_mode mode) {
	struct wish_context * new_context ;

	new_context = NULL ;

	if (!(new_context = (struct wish_context *)calloc(sizeof(struct wish_context), 1))) {
		pDEBUG("malloc error") ;
		return NULL ;
	}

	pDEBUG("pDEBUG test") ;

	*new_context = WISH_CONTEXT_DEFAULT ;

	new_context->mode = mode ;

	return new_context ;
}

void wish_context_delete(struct  wish_context ** context) ;

void wish_input_sanitize(void) {
	return ;
}

bool wish_context_get_input(struct wish_context * context) {
	// TODO stub
	return false ;	
}

bool wish_context_parse_input(struct wish_context * context) {
	// TODO stub
	return false ;
}
