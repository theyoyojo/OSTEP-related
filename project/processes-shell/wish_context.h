#ifndef WISH_CONTEXT_H
#define WISH_CONTEXT_H "wish_context.h"

#include <unistd.h>
#include <stdbool.h> /* true == success, false == !success */

#include "wish_input.h"

#define WISH_CONTEXT_ENULL 	1 	/* we expect a valid pointer but got null */
#define WISH_CONTEXT_EINPUT 	2	/* something about the input format is wonky */
#define WISH_CONTEXT_EPARM 	3	/* bad parameters to command */
#define WISH_CONTEXT_EBUG	4	/* some unknown bug has occured; this should not happen */
#define WISH_CONTEXT_EMEM	5	/* the classic error of tried to alloc but failed */
#define WISH_CONTEXT_ENOFILE	6	/* cannot open a file that we want to open */

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
	struct wish_input  	** input ;
	//struct wish_history 	history ;
	pid_t			pid ;
	int			input_fd ;
	int 			output_fd ;	
	FILE 			* input_file ;
	FILE 			* output_file ;
	char 			** pathv ;
	size_t 			pathc ;
	bool 			path_modified ;
	size_t 			size ;
	unsigned 		children ;
} ;

#define WISH_CONTEXT_DEFAULT (struct wish_context) { 		\
	.mode = 	WISH_MODE_INACTIVE, 			\
	.input =	NULL, 					\
	.pid =		getpid(), 				\
	.input_fd =	STDIN_FILENO, 				\
	.output_fd = 	STDOUT_FILENO, 				\
	.input_file = 	NULL, 					\
	.output_file = 	NULL,					\
	.pathv =	wish_context_get_default_path(), 	\
	.pathc =	1,					\
	.path_modified= false,					\
	.size = 	0,					\
	.children = 	0,					\
}

static inline int wish_context_is_active(struct wish_context * context) {
	return (int)context->mode ;
}

struct wish_context * wish_context_new(void) ;

char ** wish_context_get_default_path(void) ;

void wish_context_delete(struct wish_context ** context) ;

bool wish_context_get_input(struct wish_context * context) ;

bool wish_context_parse_input(struct wish_context * context, size_t index) ;

char * wish_context_get_token_at(struct wish_context * context, size_t index, size_t offset) ;

#endif /* #ifndef WISH_CONTEXT_H */
