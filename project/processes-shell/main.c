#include <stdio.h>
#include <string.h>

#include "wish_context.h"

void error(void) ;

int main(int argc, char * argv[]) {
	/* setup */

	struct wish_context * context = wish_context_new() ;

	while (wish_context_is_active(context)) {
		
	}

	return 0 ;
}

void error(void) {
	static char error_message[30] = "An error has occurred\n" ;
	write(STDERR_FILENO, error_message, strlen(error_message)) ;
}
