/*
 * File:	measure_context_switch.c
 * By: 	 	Joel savitz <joelsavitz@gmail.com> && https://joelsavitz.com
 * Created on: 	1 May 2019
 * Purpose:	Generate a number of timing estimations for a context switch
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sched.h>
#include <sys/wait.h>

#include "ez_tsc.h"

#include "mutex_file.h"

	/* Algorithm:
	 * 0. Create a pipe
	 * 1. Spawn two child processes
	 * 2. limit each child to the same cpu (cpu 0 for simplicity)
	 * 3. children are trained on the following regimen:
	 * 	a. record [pid]: tsc in file
	 * 	b. write to pipe, unblocking sibling
	 * 	c. record new [pid]: tsc in file
	 * 	d. read from pipe, blocking until signal from sibling
	 * 	(first child starts at a, second starts at c)
	 * 4. data is invalid if:
	 * 	a. there are not exactly two unique pid numbers recorded
	 * 	b. a tsc record from a pid must be followed by exactly
	 * 	  +0 or 1 records with the same pid
	 *	c. if a tsc record has the same pid as the previous, the
	 *	  +following record must have a different pid
	 * 5. To create context switch estimation data from raw tsc records:
	 * 	a. for every line in the tsc record file:
	 * 		i. if there is a line previously saved and pid numbers differ:
	 * 			1. Take the difference between the two tsc records
	 * 			2. write the difference to output
	 * 		ii. save the current line as the new previous line
	 *
	 * Steps 0-3 are implemented below. Steps 4 and 5 are implemented in examine_data.c
	 *
	 */

enum pipe_ops {NO_OP, READ_OP, WRITE_OP} ;

/* next operation for process */
enum pipe_ops current_pipe_op ;

/* ipc mechanism */
int pipefd_a[2], pipefd_b[2] ;

struct pipe_set {
	int readfd ;
	int writefd ;
} ;

char mutex_filename[] = "mutex_file_measure_context_switch" ;

struct pipe_set * current_pipe_set ;

/* how many context switches to measure? */
int repetitions ;
#define DEFAULT_REPITITIONS 100

/* An arbitrary message to send through the pipe */
const static char msg[] = "Hello, world!\n" ;

/* A cpu set of just cpu 0 */
cpu_set_t cpu_set_just_zero ;

/* Record the tsc and pid in @output_file */
void record_tsc(FILE * output_file) ;

/* Entry point for child to begin doing childish things */
void child_main(void) ;

/* Create a child and let it run free */
pid_t make_child(enum pipe_ops first_pipe_op, struct pipe_set child_pipe_set) ;

/* Do the actual work (should only be called from child */
void do_current_pipe_op(void) ;

/* This should be set to zero at the begining of main and should only change if there is a bug */
int debug_no_op_called ;

/* Set default values of globals */
void set_defaults(void) ;

/* The actual pipe I/O */
void do_write_pipe_op(void) ;
void do_read_pipe_op(void) ;

int main(int argc, char * argv[]) {
	pid_t child_a, child_b ;

	set_defaults() ;

	if (argc > 1) if (!(repetitions = atoi(argv[1]))) {
		fprintf(stderr, "Unable to parse integer arg1? Is it actually an integer?\n") ;
		exit(1) ;
	}
		
	/* 0. Create a pipes */
	if (pipe(pipefd_a) < 0 || pipe(pipefd_b) < 0) {
		perror("Error creating pipe") ;
	}

	/* 1. Spawn two child processes */
	child_a = make_child(WRITE_OP, (struct pipe_set){pipefd_a[0], pipefd_b[1]}) ;
	child_b = make_child(READ_OP, (struct pipe_set){pipefd_b[0], pipefd_a[1]}) ;

	printf("pid %d reads from fd: %d writes to fd: %d\n",child_a,pipefd_a[0],pipefd_b[1]) ;
	printf("pid %d reads from fd: %d writes to fd: %d\n",child_b,pipefd_b[0],pipefd_a[1]) ;


	/* 2. limit each child to the same cpu (cpu 0 for simplicity) */
	if (
	sched_setaffinity(child_a,sizeof(cpu_set_just_zero),&cpu_set_just_zero) < 0 ||
	sched_setaffinity(child_b,sizeof(cpu_set_just_zero),&cpu_set_just_zero) < 0 ){
		perror("Unable to set cpu affinity") ;
		exit(1) ;
	}

	/* Wait for children to perform step 3 */
	waitpid(child_a,NULL,0) ;
	waitpid(child_b,NULL,0) ;
	
	return 0 ;
}

void record_tsc(FILE * output_file) {
	fprintf(output_file, "%d %llu\n", getpid(), rdtsc()) ;
}

void do_write_pipe_op(void) {
	fprintf(stdout, "[WRITE]\n") ;
	write(current_pipe_set->writefd, &msg,sizeof(msg)) ;
}

void do_read_pipe_op(void) {
	char buff[sizeof(msg)] ;
	fprintf(stdout, "[READ]\n") ;
	read(current_pipe_set->readfd, buff,sizeof(msg)) ;
}

void do_current_pipe_op(void) {
	switch (current_pipe_op) {
		case READ_OP:
			record_tsc(stdout) ;
			do_read_pipe_op() ;
			current_pipe_op = WRITE_OP ;
			break ;
		case WRITE_OP:
			record_tsc(stdout) ;
			do_write_pipe_op() ;
			current_pipe_op = READ_OP ;
			break ;
		case NO_OP:
		default:
			/* bug */
			debug_no_op_called = 1 ;
			break ;
	}
}

/* The children do not return to their home when night falls */
/* i.e. the child's codepath does not return from this function */
pid_t make_child(enum pipe_ops first_pipe_op, struct pipe_set child_pipe_set) {
	pid_t fork_retval ;

	fork_retval = fork() ;

	/* Case: Bad trip */
	if (fork_retval < 0) {
		perror("Unable to fork process") ;
		return fork_retval ;
	}
	
	/* Case: I am the child. Call child_main */
	if (fork_retval == 0) {
		current_pipe_op = first_pipe_op ;
		current_pipe_set = &child_pipe_set ;

		child_main() ;

		fprintf(stderr, "child_main returned! something is very wrong...\n") ;
		exit(1) ;
	}

	/* Case: I am the parent. Go home with the pid of the newly created child */
	return fork_retval ;
}

void set_defaults(void) {
	/* Purely a debug value */
	debug_no_op_called = 0 ;

	/* Default value for current_pipe_op */
	current_pipe_op = NO_OP ;	

	/* Fallback value */
	repetitions = DEFAULT_REPITITIONS ;

	/* Initialize our single cpu invariant cpu_set_t value */
	CPU_ZERO(&cpu_set_just_zero) ;
	CPU_SET(0,&cpu_set_just_zero) ;

	/* we have no pipe set yet */
	current_pipe_set = NULL ;
}

void child_main(void) {
	/* Ensure that the child is on the correct cpu */
	int sched_getaffinity_retval, completed_repetitions ;
	cpu_set_t cpu_set_current ;
	
	//printf("begin child_main for pid: %d\n", getpid() ) ;
	
	/* Don't begin the measurement until the process is on the correct cpu */
	do {
		if (( sched_getaffinity_retval =
			sched_getaffinity(getpid(), sizeof(cpu_set_current), &cpu_set_current)) < 0) {
			perror("Unable to get current cpu affinity") ;
			return /* bug */ ;
		}
	} while (!(CPU_EQUAL(&cpu_set_current, &cpu_set_just_zero))) ;

	/*
	 * 3. children are trained on the following regimen:
	 * 	a. record [pid]: tsc in file
	 * 	b. write to pipe, unblocking sibling
	 * 	c. record new [pid]: tsc in file
	 * 	d. read from pipe, blocking until signal from sibling
	 * 	(first child starts at a, second starts at c)
	 */
	
	for(int i = 0; i < repetitions/2; ++i) {
		do_current_pipe_op() ;
	}

	exit(0) ;
}
