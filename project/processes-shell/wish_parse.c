#include "wish_parse.h"

int wish_parse(struct wish_context * context) {
	int ret ;
	char * first_token ;

	first_token = wish_context_get_token_at(context, 0) ;
	if (!first_token) {
		return -WISH_CONTEXT_EINPUT ;
	}

	if (wish_builtin_available(context)) {
		ret = wish_builtin_exec(context,
			wish_builtin_string_to_enum(first_token)) ;
	}
	else {
		pDEBUG("STUB ;;;)") ;
		ret = 0 ;
	}

	return ret ;
}

