#ifndef WISH_CONTEXT_H
#define WISH_CONTEXT_H "wish_context.h"

#include <unistd.h>

enum wish_mode {
	WISH_MODE_INACTIVE,
	WISH_MODE_BATCH,
	WISH_MODE_INTERACTIVE,
} ;

struct wish_input {
	char 			** tokens ;
	size_t 			* token_sizes ;
	size_t 			size ;
} ;


struct wish_history {
	struct wish_input 	* history ;
	size_t			size ;

} ;

struct wish_context {
	enum wish_mode 		mode ;
	struct wish_history 	history ;
	pid_t			pid ;
} ;

static inline int wish_context_is_active(struct wish_context * context) {
	return (int)context->mode ;
}

// FIXME: params?
struct wish_context * wish_context_new(void) ;

void wish_context_delete(struct  wish_context ** context) ;


#endif /* #ifndef WISH_CONTEXT_H */
