#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include "/home/joel/lil_test/src/lil_test.h"

#define L LAMBDA(int, (void){printf(" THIS PROGRAM JUST TOOK A FAT L\n"); return 1;})()
#define W 0
#define DEFAULT_STACK_CAPACITY 50
#define DEFAULT_STACK_EXPANSION_RATE 2

struct intstack {
	int * data ;
	size_t size ;
	size_t capacity ;
} ;

int stack_init(struct intstack * new) ;
int stack_kill(struct intstack * old) ;
int stack_push(struct intstack * current, int * new_item) ;
int stack_popi(struct intstack * current, int * old_item) ;
int stack_extd(struct intstack * current, size_t * how_high) ;

#endif /* ifndef STACK_H */
