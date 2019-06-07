#include "wish_builtin.h"

#include <stdlib.h>
#include <string.h>

char * wish_builtin_strings[] = {
	"",
	"exit",
	"cd",
	"path",
} ;


char * wish_builtin_enum_to_string(enum wish_builtin builtin_enum) {
	return wish_builtin_strings[(size_t)builtin_enum] ;
}

static int builtin_cd(struct wish_context * context) {
	/* we require exactly one argument */
	if (context->input->size != 2) {
		return -WISH_CONTEXT_EPARM;
	}

	return chdir(context->input->tokens[1]) ;
}
static int builtin_exit(struct wish_context * context) {
	/* we require exactly zero arguments */
	if (context->input->size > 1) {
		return -WISH_CONTEXT_EPARM;
	}
	
	context->mode = WISH_MODE_INACTIVE ;
	return 0 ;
}
static int builtin_path(struct wish_context * context) {
	size_t counter ;

	if (context->path_modified) {
		for (counter = 0; counter < context->pathc; ++ counter) {
			free(context->pathv[counter]) ;
			free(context->pathv) ; context->pathv = NULL ;
		}
	}
	
	context->pathc = context->input->size - 1 ;
	context->pathv = (char **)calloc(sizeof(char *),
		/* pathv will contain a single null pointer when empty */
		context->input->size == 1 ? 1 : context->pathc ) ;
	if (!context->pathv) {
		return -WISH_CONTEXT_EMEM ;
	}
	/* and we don't want to every actually iterate through it */
	for (counter = 0; counter < context->pathc; ++counter) {
		context->pathv[counter] = strdup(context->input->tokens[counter + 1]) ;
	}
	context->path_modified = true ;

	return 0 ;
}

static int builtin_invalid(struct wish_context * context) {
	(void)context ;
	return -WISH_CONTEXT_EBUG ;
}

enum wish_builtin wish_builtin_string_to_enum(char * builtin_string) {
	enum wish_builtin ret ;

	if (strcmp(builtin_string, "exit") == 0) {
		ret = WISH_BUILTIN_EXIT ;
	}
	else
	if (strcmp(builtin_string, "cd") == 0) {
		ret = WISH_BUILTIN_CD ;
	}
	else
	if (strcmp(builtin_string, "path") == 0) {
		ret = WISH_BUILTIN_PATH ;
	}
	else {
		ret = WISH_BUILTIN_INVALID ;
	}

	return ret ;
}

int wish_builtin_exec(struct wish_context * context, enum wish_builtin builtin) {
	int (*builtin_function)(struct wish_context *) = builtin_invalid ;

	if (!context) {
		pDEBUG("invalid context") ;
		return -WISH_CONTEXT_ENULL ;	
	}

	switch (builtin) {
		case WISH_BUILTIN_CD:
			builtin_function = builtin_cd ;
			break ;
		case WISH_BUILTIN_EXIT:
			builtin_function = builtin_exit ;
			break ;
		case WISH_BUILTIN_PATH:
			builtin_function = builtin_path ;
			break ;
		case WISH_BUILTIN_INVALID:
		default:
			break ;
	}

	return builtin_function(context) ;
}

bool wish_builtin_available(struct wish_context * context) {
	bool success_code = true ;

	if (!context) {
		pDEBUG("invalid context") ;
		success_code = false ;
	}
	else
	if (!context->input) {
		pDEBUG("invalid input in current context") ;
		success_code = false ;
	}
	else
	if (wish_builtin_string_to_enum(context->input->tokens[0]) == WISH_BUILTIN_INVALID) {
		pDEBUG("no builtin found (not an error)") ;
		success_code = false ;
	}

	return success_code ;
}
