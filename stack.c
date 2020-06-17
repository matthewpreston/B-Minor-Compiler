#include <stdlib.h>
#include "stack.h"

// Constructor for a stack
struct stack *stack_create() {
	struct stack *stack = malloc(sizeof(struct stack));
	if (stack == NULL) return NULL;
	stack->top = NULL;
	return stack;
}

// Pushes data onto stack (0 if success, 1 if malloc error, 2 if NULL given)
int stack_push(struct stack *stack, void *data) {
	if (stack == NULL) return 2;

	struct stack_node *node = malloc(sizeof(struct stack_node));
	if (node == NULL) return 1;
	node->data = data;
	
	// Add to stack
	node->prev = stack->top;
	stack->top = node;
	return 0;
}

// Returns height of stack (-1 if uninitialized)
int stack_height(struct stack *stack) {
	if (stack == NULL) return -1;	
	
	int count = 0;
	for (struct stack_node *node = stack->top; node != NULL; node = node->prev, count++);
	return count;
}

// Returns a pointer to the data of the top of the stack (NULL if uninitialized)
void *stack_top(struct stack *stack) {
	return (stack == NULL) ? NULL : stack->top->data;
}

// Gives top of stack (NULL if uninitialized)
struct stack_node *stack_traverse_start(struct stack *stack) {
	return (stack == NULL) ? NULL : stack->top;
}

// Iterates to next layer below in stack (NULL if uninitialized)
struct stack_node *stack_traverse_iter(struct stack_node *node) {
	return (node == NULL) ? NULL : node->prev;
}

// To check to see if we're at the end
struct stack_node *stack_traverse_end() {
	return NULL;
}

// Pops data from top of the stack and returns it (NULL if uninitialized or empty)
void *stack_pop(struct stack *stack) {
	if (stack == NULL || stack->top == NULL) return NULL; // Empty stack
	
	// Pop off stack
	struct stack_node *node = stack->top;
	stack->top = node->prev;
	
	// Retrieve and return data
	void *data = node->data;
	free(node);
	return data;
}

// Destructor for a stack, with a destructor to use for the data within
void stack_free(struct stack *stack, void (*data_free)(void *)) {
	if (stack == NULL) return;
	
	struct stack_node *node = stack->top, *prev;
	while (node != NULL) {
		prev = node->prev;
		
		// Free data and node itself
		(*data_free)(node->data);
		free(node);
		
		node = prev;
	}
	free(stack);
}
