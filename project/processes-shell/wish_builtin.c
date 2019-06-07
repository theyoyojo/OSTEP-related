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
	pDEBUG("STUB") ;
	return 0 ;
}
static int builtin_exit(struct wish_context * context) {
	
	context->mode = WISH_MODE_INACTIVE ;
	return 0 ;
}
static int builtin_path(struct wish_context * context) {
	pDEBUG("STUB") ;
	return 0 ;
}

static int builtin_invalid(struct wish_context * context) {
	pDEBUG("STUB but this is invalid") ;
	return 0 ;
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
