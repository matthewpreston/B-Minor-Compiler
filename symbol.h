#ifndef SYMBOL_H
#define SYMBOL_H

#include "ast.h"

// Symbol table
typedef enum {
	SYMBOL_UNINITIALIZED,
	SYMBOL_LOCAL,
	SYMBOL_PARAM,
	SYMBOL_GLOBAL
} symbol_t;

struct symbol {
	symbol_t kind;
	struct type *type;
	const char *name;
	int which;
};

// Constructor for symbols
struct symbol *symbol_create(symbol_t kind, struct type *type, const char *name);

// Destructor for symbols
void symbol_free(struct symbol *s);

#endif /* SYMBOL_H */

