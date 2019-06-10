#undef DEBUG
#include "wish_context.h"
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <linux/limits.h>

static char * default_path = "/bin" ;

char ** wish_context_get_default_path(void) {
	return &default_path ;
}


int wish_execv(char * path, char * const argv[]) {
	pid_t child_pid ;

	child_pid = fork() ;

	/* case: we send the child off into the work to seek their fortune */
	if (!child_pid) {
		execv(path, argv) ;
	}

	return wait(NULL) != -1 ;
}

// nah?
void wish_input_sanitize(void) ;

struct wish_context * wish_context_new(void) {
	struct wish_context * new_context ;

	new_context = NULL ;

	if (!(new_context = (struct wish_context *)calloc(sizeof(struct wish_context), 1))) {
		pDEBUG("malloc error") ;
		return NULL ;
	}

	*new_context = WISH_CONTEXT_DEFAULT ;

	new_context->mode = WISH_MODE_INACTIVE ;

	return new_context ;
}

void wish_context_delete(struct wish_context ** context_ptr) {
	if (context_ptr) {
		struct wish_context * context = *context_ptr ;
		if (context) {
			if (context->path_modified) {
				for (size_t counter = 0; counter < context->pathc; ++counter) {
					free(context->pathv[counter]) ;
				}
				free(context->pathv) ;
			}
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
	bool success_code = false ;
	size_t path_counter ;
	char buff[PATH_MAX] ;

	memset(buff, 0, PATH_MAX) ;

	path_counter = 0 ;
	/* printf("pathc before pathcheck: %zu", context->pathc) ; */
	while (success_code == false && path_counter < context->pathc) {
		strcpy(buff, context->pathv[path_counter]) ;
		buff[strlen(buff)] = '/' ;
		strcat(buff, wish_context_get_token_at(context, 0)) ;
		/* printf("trying to access path: %s\n", buff) ; */
		success_code = !access(buff, X_OK) ;
		++path_counter ;
	}
	
	/* if (success_code) pDEBUG("found it!") ; else pDEBUG("didn't find it :(") ; */

	/* case: input is valid and an executable is found */
	if (success_code) {
		/* u know what time it is */
		wish_execv(buff, context->input->tokens) ;
	}
	
	return success_code ;
}


char * wish_context_get_token_at(struct wish_context * context, size_t index) {
	if (index > context->input->size) {
		return NULL ;
	}

	return context->input->tokens[index] ;
}
