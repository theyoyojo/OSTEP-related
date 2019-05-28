#include "stack.h"

int stack_init(struct intstack * new) {
	if (!new) return L ;

	new->data = NULL ;
	new->capacity = DEFAULT_STACK_CAPACITY ;
	if (
		!(new) ||
		stack_extd(new,&new->capacity)
	) return L ;
	new->size = 0 ;
	
	return W ;
}
int stack_kill(struct intstack * old) {
	if(!old || !old->data) return L ;

	free(old->data) ;
	return W ;
}

int stack_push(struct intstack * current, int * new_item) {
	if (!current || !new_item) return L ;
	
	if (current->size >= current->capacity) {
		current->capacity *= DEFAULT_STACK_EXPANSION_RATE ;
		if(stack_extd(current,&current->capacity)) return L ;
	}

	current->data[current->size++] = *new_item ;
	return W ;

}
int stack_popi(struct intstack * current, int * old_item) {
	if (!current || !current->size || !old_item) return L ;

	*old_item = current->data[--current->size] ;
	return W ;

}

int stack_extd(struct intstack * current, size_t * how_high) {
	int * temp ;

	if (!current || !how_high) return L ;

	temp = (int *)realloc(current->data,sizeof(int) * *how_high) ;
	if(!temp) return L ;

	current->data = temp ;
	return W ;

}
