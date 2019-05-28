#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "ez_tsc.h"


#define DEFAULT_MAX_ITERATIONS 100
int main(int argc, char * argv[]) {
	uint64_t timer_elapsed ;
	FILE * output ;
	int counter, max_iterations ;

	max_iterations = DEFAULT_MAX_ITERATIONS ;
	if (argc > 1) {
		max_iterations = atoi(argv[1]) ;
	}

	set_timer_state(TIMER_OFF) ;
	output = stdout ;

	for(counter = 0; counter < max_iterations; ++counter) {
		toggle_timer(&timer_elapsed) ;
		time(0) ;
		toggle_timer(&timer_elapsed) ;
		fprintf(output,"%lu\n",timer_elapsed) ;
	}

	return 0 ;
}
