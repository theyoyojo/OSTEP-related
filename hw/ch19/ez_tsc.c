#include "ez_tsc.h"

#include <stdint.h>

/*
 * File:	ez_tsc.c
 * By: 	 	Joel savitz <joelsavitz@gmail.com> && https://joelsavitz.com
 * Created on: 	29 April 2019
 * Purpose:	Provide an easy interface to use the tsc on a cpu
 */

static enum timer_state timer_state ;
static uint64_t timer_buffer ;

uint64_t rdtsc(void) {
	uint64_t lo, hi ;
	__asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi) : : ) ;
	return lo | ( hi << 32 ) ;
}

void toggle_timer(uint64_t * timer_elapsed_ptr) {
	if (!timer_elapsed_ptr) return ;
	
	switch (timer_state) {
	case TIMER_OFF:
		timer_buffer = rdtsc() ;
		timer_state = TIMER_ON ;
		*timer_elapsed_ptr = 0 ;
		break ;
	case TIMER_ON:
		*timer_elapsed_ptr = rdtsc() - timer_buffer ;
		timer_state = TIMER_OFF ;
		break ;
	}
}

void set_timer_state(enum timer_state new_state) {
	timer_state = new_state ;
}
