#include <stdlib.h>
#include "hash_table.h"
#include "stack.h"
#include "ast.h"
#include "scope.h"

struct stack *GLOBAL_SCOPES; // To be initialized in other source files

// Create a global scope for the global variable "GLOBAL_SCOPES"
struct stack *initialize_GLOBAL_SCOPES() {
	struct stack *temp = stack_create();
	stack_push(temp, hash_table_create(SCOPE_BINS));
	return temp;
}

// Frees a hash table full of symbol types
void _hash_table_free(void *table) {
	hash_table_free((struct hash_table *) table, (void (*)(void *)) symbol_free);
}

// Call the destructor of the stack within GLOBAL_SCOPES
void free_GLOBAL_SCOPES() {
	stack_free(GLOBAL_SCOPES, _hash_table_free);
}

// Entered a new scope
void scope_enter() {
	stack_push(GLOBAL_SCOPES, (void *) hash_table_create(SCOPE_BINS));
}

// Left current scope
void scope_exit() {
	hash_table_free((struct hash_table *) stack_pop(GLOBAL_SCOPES), free);
}

// Return which scope level currently in (if in global scope, return 1; if in local scope, return > 1)
int scope_level() {
	return stack_height(GLOBAL_SCOPES);
}

// Adds an identifier to current scope level
void scope_bind(const char *name, struct symbol *sym) {
	struct hash_table *current_scope = (struct hash_table *) stack_top(GLOBAL_SCOPES);
	
	// Create a copy of the symbol and insert
	struct symbol *s = symbol_create(sym->kind, type_create_copy(sym->type), sym->name);
	hash_table_insert(current_scope, name, (void *) s);
}

// Looks up an identifier starting with upper most scope and descends all the way to global scope
// If unable to find, returns NULL
struct symbol *scope_lookup(const char *name) {
	struct hash_table *scope;
	struct symbol *symbol;
	
	struct stack_node *iter = stack_traverse_start(GLOBAL_SCOPES);
	for (; iter != stack_traverse_end(); iter = stack_traverse_iter(iter)) {
		scope = (struct hash_table *) iter->data;
		
		if ((symbol = (struct symbol *) hash_table_search(scope, name)) != NULL)
			return symbol;
	}
	return NULL;
}

// Looks up an identifier only in current scope
// If unable to find, returns NULL
struct symbol *scope_lookup_current(const char *name) {
	struct hash_table *scope = stack_top(GLOBAL_SCOPES);
	return (struct symbol *) hash_table_search(scope, name);
}

// Returns a symbol_t regarding if the current scope level is the global scope/local scope
symbol_t is_global_scope() {
	return (scope_level() > 1) ? SYMBOL_LOCAL : SYMBOL_GLOBAL;
}
