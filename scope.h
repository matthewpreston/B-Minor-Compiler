#ifndef SCOPE_H
#define SCOPE_H

#include "symbol.h"

// Scope levels will be a series of hash tables pushed onto a stack, with the lowermost level
// being the global scope.
// Note: should be initialized using initialize_GLOBAL_SCOPES()
extern struct stack *GLOBAL_SCOPES;

// Used for determining the size of the hash tables
#ifndef SCOPE_BINS
#define SCOPE_BINS 30
#endif /* SCOPE_BINS */

// Create a global scope for the global variable "GLOBAL_SCOPES"
struct stack *initialize_GLOBAL_SCOPES();

// Call the destructor of the stack within GLOBAL_SCOPES
void free_GLOBAL_SCOPES();

// Entered a new scope
void scope_enter();

// Left current scope
void scope_exit();

// Return which scope level currently in (if in global scope, return 0; if in local scope, return > 0)
int scope_level();

// Adds an identifier to current scope level
void scope_bind(const char *name, struct symbol *sym);

// Looks up an identifier starting with upper most scope and descends all the way to global scope
// If unable to find, returns NULL
struct symbol *scope_lookup(const char *name);

// Looks up an identifier only in current scope
// If unable to find, returns NULL
struct symbol *scope_lookup_current(const char *name);

// Helper function
// Returns a symbol_t regarding if the current scope level is the global scope/local scope
symbol_t is_global_scope();

#endif /* SCOPE_H */

