#ifndef EZ_TSC_H
#define EZ_TSC_H
#include <stdint.h>
#include <stdio.h>

enum timer_state {TIMER_OFF, TIMER_ON} ;

uint64_t rdtsc(void) ;

void toggle_timer(uint64_t * timer_elapsed_ptr) ;

void set_timer_state(enum timer_state new_state) ;

#endif /* #ifndef EZ_TSC_H */
