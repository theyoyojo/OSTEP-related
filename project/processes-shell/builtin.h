#ifndef BUILTIN_H
#define BUILTIN_H "builtin.h"

char * builtin_command_strings[] = {
	"exit",
	"cd",
	"path",
} ;

enum builtin_command {
	BUILTIN_COMAND_EXIT,
	BUILTIN_COMMAND_CD,
	BUILTIN_COMMAND_PATH,
} ;

int builtin_command_exec(enum builtin_command) ;

#endif /* #ifndef BUILTIN_H */
