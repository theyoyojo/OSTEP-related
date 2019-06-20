#include "wish_context.h"
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <linux/limits.h>

static char * default_path = "/bin" ;

char ** wish_context_get_default_path(void) {
	return &default_path ;
}

/* interesting note: input->tokens[input->size] == NULL should be invariant */

char * wish_parse_redirection(struct wish_input * input) {	
	size_t counter ;
	for (counter = 0; counter < input->size; ++counter) {
		if (strcmp(input->tokens[counter], ">") == 0) {
			pDEBUG("found the >") ;
			if (input->tokens[counter + 1] != NULL && 
					input->size <= counter + 2
					) { /* nothing else after > target */
				pDEBUG("found the filename after >") ;
				/* case: '>' is not the last token */
				input->tokens[counter] = NULL ;
				//printf("I am child redirection to %s\n", input->tokens[counter + 1]) ;
				return input->tokens[counter + 1] ;
			}
			else  {
				pDEBUG("darn the beef is dead") ;
				return (char *)0xdeadbeef ; /* no filename idk its a hack */
			}
			pDEBUG("wat") ;
		}
	}

	pDEBUG("leaving") ;
	return NULL ;
}



int wish_execv(struct wish_context * context, size_t index, char * path, char * const argv[]) {
	pid_t child_pid ;
	char * output_filename = wish_parse_redirection(context->input[index]) ;
	FILE * output_file = NULL ;

	if (output_filename == (char *)0xdeadbeef)	 {
		/* hacked in case: no filename after > */
		pDEBUG("no filename after >") ;
		return -WISH_CONTEXT_EPARM ;
	}
	else if (output_filename) {
		output_file = fopen(output_filename, "w") ;
		if (!output_file) {
			pDEBUG("problem opening file") ;
			return -WISH_CONTEXT_EBUG ; /* yeah idk it is a bug */
		}
	}

	/***** THE FORK *****/
	child_pid = fork() ;

	if (child_pid < 0) {
		/* case: ??? */
		return -WISH_CONTEXT_EBUG ;
	}
	else
	if (child_pid > 0) {
		++context->children ;
	}

	/* case: we send the child off into the world to seek their fortune */
	if (!child_pid) {
		if (output_file) {
			pDEBUG("child is redirecting...") ;
			dup2(fileno(output_file), STDOUT_FILENO) ;		
			dup2(fileno(output_file), STDERR_FILENO) < 0?pDEBUG("syscall fail"):pDEBUG("yeet") ;
		}
		pDEBUG("exec...........") ;
		execv(path, argv) ;
	}

	return 0 ;
}

// nah?
void wish_input_sanitize(void) ;

#define WISH_CONTEXT_INPUT_BUFFSIZE 1024 /* thicc buffer */
struct wish_context * wish_context_new(void) {
	struct wish_context * new_context ;

	new_context = NULL ;

	if (!(new_context = (struct wish_context *)calloc(sizeof(struct wish_context), 1))) {
		pDEBUG("malloc error") ;
		return NULL ;
	}

	*new_context = WISH_CONTEXT_DEFAULT ;
	if (!(new_context->input = (struct wish_input **)calloc(sizeof(struct wish_input *),
					WISH_CONTEXT_INPUT_BUFFSIZE))) {
		pDEBUG("baaaaad allllooooc nooo") ;
	}

	new_context->mode = WISH_MODE_INACTIVE ;

	return new_context ;
}

/* precondition: valid fds */
int wish_context_fdopen(struct wish_context * context) {
	context->input_file = fdopen(context->input_fd, "r") ;	
	context->output_file = fdopen(context->output_fd, "w") ;	

	return -(!context->input_file || !context->output_file) ;
}

int wish_context_fclose(struct wish_context * context) {
	if (context->input_file) fclose(context->input_file) ;
	if (context->output_file) fclose(context->output_file) ;

	return 0 ;
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
			for (size_t counter = 0; counter < context->size; ++counter) {
				wish_input_delete(context->input[counter]) ;
			}
			free(context->input) ;
			/* file business */
			wish_context_fclose(context) ;

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
	struct wish_input * (*get_input)(FILE * input_file) ;

	/* active files if not already done */
	if (!context->input_file || !context->output_file) {
		if (wish_context_fdopen(context) < 0) {
			pDEBUG("cannot open fds as files oops") ;
			return false ;
		}
	}

	/* if we have old input, delete it and get new input */
	if (context->input) {
		for (size_t counter = 0; counter < context->size; ++counter) {
			wish_input_delete(context->input[counter]) ;
		} 
		context->size = 0 ;
	}

	switch (context->mode) {
		case WISH_MODE_INTERACTIVE:
			get_input = wish_input_new_interactive ;
			break ;
		case WISH_MODE_BATCH:
		default:
			get_input = wish_input_new ;
			break ;
	}

	return !!(context->input[context->size++] = get_input(context->input_file)) ;
}

/* executed when no builtins are available */
bool wish_context_parse_input(struct wish_context * context, size_t index) {
	bool success_code = false ;
	size_t path_counter ;
	char buff[PATH_MAX] ;


	path_counter = 0 ;
	/* printf("pathc before pathcheck: %zu", context->pathc) ; */
	while (success_code == false && path_counter < context->pathc) {
		memset(buff, 0, PATH_MAX) ;
		strcpy(buff, context->pathv[path_counter]) ;
		buff[strlen(buff)] = '/' ;
		strcat(buff, wish_context_get_token_at(context, index, 0)) ;
		/* printf("trying to access path: %s\n", buff) ; */
		success_code = !access(buff, X_OK) ;
		++path_counter ;
	}
	
	if (success_code) pDEBUG("found it!") ; else pDEBUG("didn't find it :(") ;

	/* case: input is valid and an executable is found */
	if (success_code) {
		/* u know what time it is */
		success_code = wish_execv(context, index, buff, context->input[index]->tokens) == 0 ;
		/* int -> bool code */
	}
	
	return success_code ;
}


char * wish_context_get_token_at(struct wish_context * context, size_t index, size_t offset) {
	if (offset > context->input[index]->size) {
		return NULL ;
	}

	return context->input[index]->tokens[offset] ;
}
