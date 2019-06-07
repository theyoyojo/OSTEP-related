#include "wish_parse.h"

int wish_parse(struct wish_context * context) {
	int ret ;
	char * first_token ;

	/* what kind of nonsense does the user want the computer to attempt */
	first_token = wish_context_get_token_at(context, 0) ;

	/* case: no idea */
	if (!first_token) {
		return -WISH_CONTEXT_EINPUT ;
	}
	else
	/* case: if there is a builtin available for that command, run it */
	if (wish_builtin_available(context)) {
		ret = wish_builtin_exec(context,
			wish_builtin_string_to_enum(first_token)) ;
	}
	/* case: try and find an executable in the specified paths */
	else {
		/* bool code to int code (nonzero ret on success -> zero ret on success) */
		ret = !wish_context_parse_input(context) ;
	}

	return -ret ;
}

