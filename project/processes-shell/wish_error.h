#ifndef WISH_ERROR_H
#define WISH_ERROR_H

#include <unistd.h>
#include <string.h>

static inline void error(void) {
	static char error_message[30] = "An error has occurred\n" ;
	write(STDERR_FILENO, error_message, strlen(error_message)) ;
}

#endif /* #ifndef WISH_ERROR_H */
