#include <stdlib.h>

int main(void) {
	char * x ;
	x = (char *)malloc(101) ;

	x[112] = '\0' ;

	free(x) ;

	return 0 ;
}
