#include <stdlib.h>
#include "decl.h"

// Constructor for type
struct type *type_create(
		type_t kind,
		struct type *subtype,
		struct param_list *params)
{
	struct type *t = malloc(sizeof(struct type));
	t->kind = kind;
	t->subtype = subtype;
	t->params = params;
	return t;
}

// Destructor for type
void type_free(struct type *t) {
	if (t == NULL) return;
	if (t->subtype != NULL) type_free(t->subtype);
	if (t->params  != NULL) param_list_free(t->params);
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

// Destructor for parameter list
void param_list_free(struct param_list *p) {
	if (p == NULL) return;
	if (p->type != NULL) type_free(p->type);
	if (p->next != NULL) param_list_free(p->next);
	free(p);
}

// Constructor for declarations
struct decl *decl_create(
		const char *name,
		struct type *type,
		struct expr *value,
		struct stmt *code,
		struct decl *next)
{
	struct decl *d = malloc(sizeof(struct decl));
	d->name = name;
	d->type = type;
	d->value = value;
	d->code = code;
	d->next = next;
	return d;
}

// Destructor for declarations
void decl_free(struct decl *d) {
	if (d == NULL) return;
	if (d->type  != NULL) type_free(d->type);
	if (d->value != NULL) expr_free(d->value);
	if (d->code  != NULL) stmt_free(d->code);
	if (d->next  != NULL) decl_free(d->next);
	free(d);
}

// Constructor for statements
struct stmt *stmt_create(
		stmt_t kind,
		struct decl *decl,
		struct expr *init_expr,
		struct expr *expr,
		struct expr *next_expr,
		struct stmt *body,
		struct stmt *else_body,
		struct stmt *next)
{
	struct stmt *s = malloc(sizeof(struct stmt));
	s->kind = kind;
	s->decl = decl;
	s->init_expr = init_expr;
	s->expr = expr;
	s->next_expr = next_expr;
	s->body = body;
	s->else_body = else_body;
	s->next = next;
	return s;
}

// Destructor for statements
void stmt_free(struct stmt *s) {
	if (s == NULL) return;
	if (s->decl 	 != NULL) decl_free(s->decl);
	if (s->init_expr != NULL) expr_free(s->init_expr);
	if (s->expr 	 != NULL) expr_free(s->expr);
	if (s->next_expr != NULL) expr_free(s->next_expr);
	if (s->body 	 != NULL) stmt_free(s->body);
	if (s->else_body != NULL) stmt_free(s->else_body);
	if (s->next 	 != NULL) stmt_free(s->next);
	free(s);
}

// Constructor for expressions
struct expr *expr_create(expr_t kind, struct expr *left, struct expr *right) {
	struct expr *e = malloc(sizeof(struct expr));
	e->kind = kind;
	e->left = left;
	e->right = right;
	return e;
}

// Factory constructors for expressions
struct expr *expr_create_identifier(const char *name) {
	struct expr *e = malloc(sizeof(struct expr));
	e->kind = EXPR_IDENTIFIER;
	e->left = NULL;
	e->right = NULL;
	e->name = name;
	return e;
}

struct expr *expr_create_integer_literal(int i) {
	struct expr *e = malloc(sizeof(struct expr));
	e->kind = EXPR_INTEGER_LITERAL;
	e->left = NULL;
	e->right = NULL;
	e->integer_value = i;
	return e;
}

struct expr *expr_create_char_literal(char c) {
	struct expr *e = malloc(sizeof(struct expr));
	e->kind = EXPR_CHAR_LITERAL;
	e->left = NULL;
	e->right = NULL;
	e->integer_value = (int) c;
	return e;
}

struct expr *expr_create_string_literal(const char *str) {
	struct expr *e = malloc(sizeof(struct expr));
	e->kind = EXPR_STRING_LITERAL;
	e->left = NULL;
	e->right = NULL;
	e->string_literal = str;
	return e;
}

struct expr *expr_create_boolean_literal(int b) {
	struct expr *e = malloc(sizeof(struct expr));
	e->kind = EXPR_BOOLEAN_LITERAL;
	e->left = NULL;
	e->right = NULL;
	e->integer_value = b;
	return e;
}

// Destructor for expressions
void expr_free(struct expr *e) {
	if (e == NULL) return;
	if (e->left  != NULL) expr_free(e->left);
	if (e->right != NULL) expr_free(e->right);
	free(e);
}
