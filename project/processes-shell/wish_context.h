#ifndef WISH_CONTEXT_H
#define WISH_CONTEXT_H "wish_context.h"

#include <unistd.h>
#include <stdbool.h> /* true == success, false == !success */

#include "wish_input.h"

#define WISH_CONTEXT_ENULL 1
#define WISH_CONTEXT_EINPUT 2

enum wish_mode {
	WISH_MODE_INACTIVE = 0,
	WISH_MODE_BATCH,
	WISH_MODE_INTERACTIVE,
} ;

// TODO
/* struct wish_history { */
/* 	struct wish_input 	* history ; */
/* 	size_t			size ; */
/* } ; */

struct wish_context {
	enum wish_mode 		mode ;
	struct wish_input * 	input ;
	//struct wish_history 	history ;
	pid_t			pid ;
	int			input_fd ;
	int 			output_fd ;	
} ;

#define WISH_CONTEXT_DEFAULT (struct wish_context) { \
	.mode = 	WISH_MODE_INACTIVE, \
	.input =	NULL, \
	.pid =		getpid(), \
	.input_fd =	STDIN_FILENO, \
	.output_fd = 	STDOUT_FILENO, \
}

static inline int wish_context_is_active(struct wish_context * context) {
	return (int)context->mode ;
}

// FIXME: params?
struct wish_context * wish_context_new(enum wish_mode mode) ;

void wish_context_delete(struct  wish_context ** context) ;

bool wish_context_get_input(struct wish_context * context) ;

bool wish_context_parse_input(struct wish_context * context) ;

char * wish_context_get_token_at(struct wish_context * context, size_t index) ;

#endif /* #ifndef WISH_CONTEXT_H */
