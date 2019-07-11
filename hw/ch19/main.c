#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include "ez_tsc.h"

#define GIGABYTE (1UL << 30)
#define MEGABYTE (1UL << 20)

unsigned long page_size ;

void panic(void) {
	printf("Bruh moment -- killing self\n") ;
	exit(666) ;
}

void usage(void) {
	printf("tlbmeasure <pages> <reps>\n") ;
}

uint64_t take_measurement(size_t num_pages, char * a) {
	size_t i ;
	uint64_t result ;


	toggle_timer(&result) ;
	for (i = 10 ; i < num_pages * page_size; i += page_size) {
		++a[i] ;
	}
	toggle_timer(&result) ;

	//printf("result = %lu\n", result) ;


	return result ;
}

uint64_t average_result(size_t pages, size_t reps) {
	if (!reps) panic() ;

	size_t i ;
	uint64_t sum ;

	char * a = malloc(pages * page_size) ;
	a ? memset(a,0, pages * page_size) : panic() ;

	sum = 0 ;
	for (i = 0; i < reps; ++i) {
		sum += take_measurement(pages, a) ;
	}

	free(a) ;
	return sum / reps ; 
}

int main(int argc, char * argv[]) {
	size_t reps, pages ;

	cpu_set_t cpuset ;
	CPU_ZERO(&cpuset) ;
	CPU_SET(0, &cpuset) ;
	sched_setaffinity(getpid(), sizeof(cpuset), &cpuset) ;

	page_size = getpagesize() ;
	set_timer_state(TIMER_OFF) ;

	if (argc != 3) {
		usage() ;
		exit(1) ;
	}

	pages = atoi(argv[1]) ;
	reps = atoi(argv[2]) ;

	//printf("Page size is %luKiB\n", page_size >> 10) ;

	printf("%zu pages x %zu reps:\t  %lu cycles on cpu 0\n", pages, reps,
		average_result(pages, reps)) ;

	return 0 ;
	
}
