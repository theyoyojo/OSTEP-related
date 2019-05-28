#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

int main(int argc, char * argv[]) {
	struct intstack stack ;
	int input;
	size_t counter ;
	double sum ;

	(void)argc, (void)argv ;

	if(stack_init(&stack)) return L ;

	while (scanf("%d",&input) != EOF) stack_push(&stack, &input) ;

	for(counter = 0; counter < stack.size; ++counter) sum += stack.data[counter] ;

	printf("%lg\n",sum/stack.size) ;

	stack_kill(&stack) ;

	return W ;

}
