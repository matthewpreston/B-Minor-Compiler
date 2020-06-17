#ifndef STACK_H
#define STACK_H

struct stack_node {
	void *data;
	struct stack_node *prev;
};

struct stack {
	struct stack_node *top;
};

// Constructor for a stack
struct stack *stack_create();

// Pushes data onto stack (0 if success, 1 if malloc error, 2 if NULL given)
int stack_push(struct stack *stack, void *data);

// Returns height of stack (-1 if uninitialized)
int stack_height(struct stack *stack);

// Returns a pointer to the data of the top of the stack (NULL if uninitialized)
void *stack_top(struct stack *stack);

// For traversal through stack, similar to ::iterator in C++
// Usage:
// struct stack_node *iter;
// for (iter = stack_traverse_start(stack); iter != stack_traverse_end();
//		iter = stack_traverse_iter(iter)) {
//     do_something(iter->data, ...);
// }

// Gives top of stack (NULL if uninitialized)
struct stack_node *stack_traverse_start(struct stack *stack);

// Iterates to next layer below in stack (NULL if uninitialized)
struct stack_node *stack_traverse_iter(struct stack_node *node);

// To check to see if we're at the end
struct stack_node *stack_traverse_end();

// Pops data from top of the stack and returns it (NULL if uninitialized or empty)
void *stack_pop(struct stack *stack);

// Destructor for a stack, with a destructor to use for the data within
void stack_free(struct stack *stack, void (*data_free)(void *));

#endif /* STACK_H */

