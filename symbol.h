#ifndef SYMBOL_H
#define SYMBOL_H

#include "ast_type.h"

// Symbol table
typedef enum {
	SYMBOL_LOCAL,
	SYMBOL_PARAM,
	SYMBOL_GLOBAL
} symbol_t;

struct symbol {
	symbol_t kind;
	const char *name;
	struct type *type;
	int which;
};

// Constructor for symbols
struct symbol *symbol_create(symbol_t kind, const char *name, struct type *type);

// Destructor for symbols
void symbol_free(struct symbol *s);

#endif /* SYMBOL_H */

