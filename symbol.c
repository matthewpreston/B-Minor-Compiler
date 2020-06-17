#include <stdlib.h>
#include "symbol.h"

// Constructor for symbols
struct symbol *symbol_create(symbol_t kind, struct type *type, const char *name) {
	struct symbol *s = malloc(sizeof(struct symbol));
	s->kind = kind;
	s->type = type;
	s->name = name;
	return s;
}

// Destructor for symbols
void symbol_free(struct symbol *s) {
	if (s == NULL) return;
	if (s->type != NULL) type_free(s->type);
	free(s);
}
