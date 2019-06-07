#ifndef WISH_BUILTIN_H
#define WISH_BUILTIN_H "wish_builtin.h"

#include "wish_context.h"

enum wish_builtin {
	WISH_BUILTIN_INVALID,
	WISH_BUILTIN_EXIT,
	WISH_BUILTIN_CD,
	WISH_BUILTIN_PATH,
} ;

/* context-dependent */

/* return zero for success, nonzero for error */
int wish_builtin_exec(struct wish_context * context, enum wish_builtin builtin) ;

bool wish_builtin_available(struct wish_context * context) ;

/* context-independent */
char * wish_builtin_enum_to_string(enum wish_builtin builtin) ;
enum wish_builtin wish_builtin_string_to_enum(char * builtin_string) ;

#endif /* #ifndef WISH_BUILTIN_H */
