#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast_type.h"
#include "scope.h"
#include "stack.h"
#include "symbol.h"

// Constructor for type
struct type *type_create_basic(type_t kind) {
	struct type *t = malloc(sizeof(struct type));
	t->kind = kind;
	t->subtype = NULL;
	t->params = NULL;
	return t;
}

struct type *type_create_array(struct type *subtype, int num_elems) {
	struct type *t = malloc(sizeof(struct type));
	t->kind = TYPE_ARRAY;
	t->subtype = subtype;
	t->num_elems = num_elems;
	return t;
}

struct type *type_create_function(struct type *subtype, struct param_list *params) {
	struct type *t = malloc(sizeof(struct type));
	t->kind = TYPE_FUNCTION;
	t->subtype = subtype;
	t->params = params;
	return t;
}

// Copy constructor for type
struct type *type_create_copy(struct type *t) {
	if (t == NULL) return NULL;

	// We must recursively make copies of the subtypes with the last being done first
	// First, we push all the subtypes onto a stack (first element is the given param, rest
	// are its subtypes)
	struct type *tail;
	struct stack *s = stack_create();
	for (tail = t; tail != NULL; tail = tail->subtype) {
		stack_push(s, (void *) tail);
	}
	
	// Then we pop off elements from the stack and create a new type, keeping this new type
	// to be the new "subtype" for the next element off the stack
	struct type *prev_subtype = NULL;
	while ((tail = (struct type *) stack_pop(s)) != NULL) {
		if (tail->kind == TYPE_ARRAY) {
			prev_subtype = type_create_array(prev_subtype, tail->num_elems);
		} else if (tail->kind == TYPE_FUNCTION) {
			prev_subtype = type_create_function(prev_subtype,
												param_list_create_copy(tail->params));
		} else {
			prev_subtype = type_create_basic(tail->kind);
		}
	}
	
	// Finally, return the final type
	stack_free(s, free);
	return prev_subtype;
}

// Returns 0 if not equal, 1 if equal. Does not check if array bounds are equal
int type_equals(struct type *a, struct type *b) {
	// Using this strategy for testing NULL pointers fails if we are comparing
	// function pointers, which one could be NULL
	//if (a == NULL || b == NULL) return (a == b);
	
	if (a == NULL) {
		// Allow functions and pointers to NULL be of the same type
		if (b != NULL) {
			return (b->kind == TYPE_FUNCTION);
		} else {
			return 1; // Both 'a' and 'b' are NULL
		}
	}
	
	if (b == NULL) {
		// Allow functions and pointers to NULL be of the same type
		if (a != NULL) {
			return (a->kind == TYPE_FUNCTION);
		} else {
			return 1; // Both 'a' and 'b' are NULL
		}
	}
	
	// Now since 'a' and 'b' are both not NULL, check types
	if (a->kind == b->kind) {
		switch (a->kind) {
			case TYPE_VOID:		// Atomic types
			case TYPE_BOOLEAN:
			case TYPE_CHAR:
			case TYPE_INTEGER:
			case TYPE_STRING:
				return 1;
				break;
			case TYPE_ARRAY:	// See if the array's contents are the same
				return type_equals(a->subtype, b->subtype);
				break;
			case TYPE_FUNCTION: // See if return type and param types are same
				// Check return type
				if (!type_equals(a->subtype, b->subtype)) return 0;
				
				// Check param types
				struct param_list *p, *q;
				for (p = a->params, q = b->params;
						p != NULL && q != NULL;
						p = p->next, q = q->next) {
					if (!type_equals(p->type, q->type)) {
						return 0;
					}
				}
				return (p == q); // Returns 1 if they're both NULL
				break;
			default:
				break;
		}
	}
	return 0;
}

// Used for checking two array types are of the same array sizes.
// Returns 0 if not equal, 1 if equal, -1 if at least one is not an array.
int type_equals_array_bounds(struct type *a, struct type *b) {
	if (a == NULL || b == NULL || a->kind != TYPE_ARRAY || b->kind != TYPE_ARRAY) return -1;
	
	return (a->num_elems == b->num_elems);
}

// Returns what type 't' is in a newly allocated C string. If NULL, returns "NULL"
char *type_stringify(struct type *t) {
	if (t == NULL) {
		return strcpy(malloc(strlen("NULL") + 1), "NULL");
	}

	int num_params;
	char *result, *temp, *temp2;
	
	switch (t->kind) {
		case TYPE_VOID:
			result = malloc(strlen("void") + 1);
			return strcpy(result, "void");
		case TYPE_BOOLEAN:
			result = malloc(strlen("boolean") + 1);
			return strcpy(result, "boolean");
		case TYPE_CHAR:
			result = malloc(strlen("char") + 1);
			return strcpy(result, "char");
		case TYPE_INTEGER:
			result = malloc(strlen("integer") + 1);
			return strcpy(result, "integer");
		case TYPE_STRING:
			result = malloc(strlen("string") + 1);
			return strcpy(result, "string");
		case TYPE_ARRAY:
			// Return the array content type as well
			temp = type_stringify(t->subtype);
			result = malloc(strlen("array [] ") + strlen(temp) + strlen("\0"));
			sprintf(result, "array [] %s", temp);
			free(temp);
			return result;
		case TYPE_FUNCTION:
			// First, get the types of the function parameters
			num_params = 0;
			for (struct param_list *p = t->params; p != NULL; p = p->next, num_params++) {
				if (num_params == 0) {
					result = type_stringify(p->type);
				} else {
					temp = type_stringify(p->type);
					temp2 = malloc(strlen(result) + strlen(", ") + strlen(temp) + 1);
					sprintf(temp2, "%s, %s", result, temp);
					free(temp);
					result = temp2;
				}
			}
			if (num_params == 0) {
				result = malloc(sizeof(char));
				*result = '\0';
			}
			
			// Then get the return type and return the final result
			temp = type_stringify(t->subtype);
			temp2 = malloc(strlen("function : ") + strlen(temp)
						   + strlen(" (") + strlen(result) + strlen(")\0"));
			sprintf(temp2, "function : %s (%s)", temp, result);
			free(temp);
			return temp2;
		default:
			break;
	}
	
	return NULL;
}

// Destructor for type
void type_free(struct type *t) {
	if (t == NULL) return;
	if (t->subtype != NULL) type_free(t->subtype);
	if (t->kind == TYPE_FUNCTION && t->params != NULL) param_list_free(t->params);
	free(t);
}

// Constructor for parameter list
struct param_list *param_list_create(
		const char *name,
		struct type *type,
		struct param_list *next)
{
	struct param_list *p = malloc(sizeof(struct param_list));
	p->name = name;
	p->type = type;
	p->next = next;
	return p;
}

// Copy constructor for parameter list
struct param_list *param_list_create_copy(struct param_list *p) {
	if (p == NULL) return NULL;
	
	// We must recursively make copies of the parameters with the last being done first
	// First, we push all the params onto a stack (first element is the given param, rest
	// are the next params)
	struct stack *s = stack_create();
	struct param_list *tail;
	for (tail = p; tail != NULL; tail = tail->next) {
		stack_push(s, (void *) tail);
	}
	
	// Then we pop off elements from the stack and create a new param, keeping this new param
	// to be the next "param" for the next element off the stack
	struct param_list *next_param = NULL;
	while ((tail = (struct param_list *) stack_pop(s)) != NULL) {
		next_param = param_list_create(tail->name,
									   type_create_copy(tail->type),
									   next_param);
	}
	
	// Finally, return the final parameter list
	stack_free(s, free);
	return next_param;
	
}

// Name resolving during AST re-traversal for parameter lists
// i.e. (have identifiers available for function code)
int param_list_resolve(struct param_list *p) {
	if (p == NULL) return 0;
	
	scope_bind(p->name, symbol_create(SYMBOL_PARAM, p->name, p->type));
	return param_list_resolve(p->next);
}

// Destructor for parameter list
void param_list_free(struct param_list *p) {
	if (p == NULL) return;
	if (p->type != NULL) type_free(p->type);
	if (p->next != NULL) 		 param_list_free(p->next);
	free(p);
}
