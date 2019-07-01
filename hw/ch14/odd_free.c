#include <stdlib.h>

int main(void) {
	char * x ;
	x = (char *)malloc(101) ;

	x[21200000] = '\0' ;

	free(x) ;

	return 0 ;
}
