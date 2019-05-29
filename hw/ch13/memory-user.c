#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define REEE_LENGTH 	100
#define ARBITRARY_BYTE 	0xaa
#define B_PER_MB 	1024 * 1024

void usage(void) {
	printf("Requires exactly one integer argument specifying the number of bytes to allocate\n") ;
}

int main(int argc, char * argv[]) {
	size_t bytes, megabytes, counter ;
	char * arr ;

	if (argc != 2) {
		usage() ;
		exit(1) ;
	}

	megabytes = atoi(argv[1]) ;

	bytes = megabytes * B_PER_MB ;

	arr = malloc(bytes) ;
	if (!arr) {
		printf("memory error ahhhhh can't malloc! Quittttinngggggg\n") ;
		putchar('r') ;
		for (counter = 0; counter < REEE_LENGTH; ++counter) {
			putchar('e') ;
		}
		exit(2) ;
	}
	else {
		printf("[pid: %d] alloc %zd megabytes OK.\n", getpid(), megabytes) ;
	}

	/* Just do a bunch of useless calculations */
	for (counter = 0 ; 1 ; counter = (counter + 1) % bytes) {
		arr[counter] ^= ARBITRARY_BYTE ;
	}

	return 0 ;
}
