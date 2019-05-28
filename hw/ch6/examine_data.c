#include <stdio.h>
/*
 * File:	examine_data.c
 * By: 	 	Joel Savitz <joelsavitz@gmail.com> && https://joelsavitz.com
 * Created on: 	1 May 2019
 * Purpose:	Implement steps 4 and 5 of algorithm described in measure_context_switch.c
 */

	/* Taken from measrure_context_switch.c:
	 *
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
	 */

int main(void) {
	int current_pid, prev_pid ;
	unsigned long long int current_tsc, prev_tsc ;
	FILE * output_file ;

	/* bogus initial values */
	current_pid = prev_pid = 0 ;
	current_tsc = prev_tsc = 0 ;
	
	/* can't hurt to be explicit */
	output_file = stdout ;

	/* I'm just going to assume the data is valid as a precondition */

	while (scanf("%d", &current_pid) != EOF) {
		scanf("%llu",&current_tsc) ;
		
		/* Case: the first line of input */
		if (prev_pid == 0 || prev_tsc == 0) {
			/* no-op */
		}
		/* Case: the second or subsequent line of input */
		else {
			if (prev_tsc >= current_tsc) printf("!!!\n") ;
			printf("%d follows %d ", current_pid ,prev_pid) ;
			printf("  %llu - %llu = ", current_tsc, prev_tsc ) ;
			printf("%llu\n", current_tsc - prev_tsc) ;
		}

		/* Current values become previous values to make room for next values */
		prev_pid = current_pid ;
		prev_tsc = current_tsc ;
	}
}
