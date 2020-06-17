#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

void sanity_check(struct stack *stack) {
	struct stack_node *node;
	
	printf("Stack (%d tall):\n", stack_height(stack));
	for (node = stack->top; node != NULL; node = node->prev) {
		printf("%d\n", *((int *) node->data));
	}
}

int main(int argc, char *argv[]) {
	struct stack *stack = stack_create();
	
	void *v1 = malloc(sizeof(int)); *((int *) v1) = 5;
	void *v2 = malloc(sizeof(int)); *((int *) v2) = 32;
	void *v3 = malloc(sizeof(int)); *((int *) v3) = 84932;
	void *v4 = malloc(sizeof(int)); *((int *) v4) = 84932;
	
	printf("Initialization\n");
	sanity_check(stack);
	
	printf("\nPushing\n");
	printf("Adding v: %d\n", *((int *) v1)); stack_push(stack, v1); sanity_check(stack);
	printf("Adding v: %d\n", *((int *) v2)); stack_push(stack, v2); sanity_check(stack);
	printf("Adding v: %d\n", *((int *) v3)); stack_push(stack, v3); sanity_check(stack);
	printf("Adding v: %d\n", *((int *) v4)); stack_push(stack, v4); sanity_check(stack);
	
	printf("\nIterating over\n");
	struct stack_node *iter = stack_traverse_start(stack);
	for (; iter != stack_traverse_end(); iter = stack_traverse_iter(iter)) {
		printf("%d\n", *((int *) iter->data));
	}
	
	printf("\nPopping\n");
	printf("Popped v: %d\n", *((int *) stack_pop(stack))); sanity_check(stack);
	printf("Popped v: %d\n", *((int *) stack_pop(stack))); sanity_check(stack);
	printf("Popped v: %d\n", *((int *) stack_pop(stack))); sanity_check(stack);
	
	printf("\nClean-up\n");
	stack_free(stack, free); // Also frees v1 since we didn't pop it off stack
	free(v2);
	free(v3);
	free(v4);
	
	printf("Done\n");
	return 0;
}
