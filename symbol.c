#include <stdlib.h>
#include "symbol.h"

// Constructor for symbols
struct symbol *symbol_create(symbol_t kind, const char *name, struct type *type) {
	struct symbol *s = malloc(sizeof(struct symbol));
	s->kind = kind;
	s->name = name;
	s->type = type;
	return s;
}

// Destructor for symbols
void symbol_free(struct symbol *s) {
	if (s == NULL) return;
	if (s->type != NULL) type_free(s->type);
	free(s);
}
