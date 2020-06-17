#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "scope.h"
#include "stack.h"

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

// Copy constructor for type
struct type *type_create_copy(struct type *t) {
	if (t == NULL) return NULL;

	// We must recursively make copies of the subtypes with the last being done first
	// First, we push all the subtypes onto a stack (first element is the given param, rest
	// are its subtypes)
	struct stack *s = stack_create();
	struct type *tail;
	for (tail = t; tail->subtype != NULL; tail = tail->subtype) {
		stack_push(s, (void *) tail);
	}
	
	// Then we pop off elements from the stack and create a new type, keeping this new type
	// to be the new "subtype" for the next element off the stack
	struct type *prev_subtype = NULL;
	while ((tail = (struct type *) stack_pop(s)) != NULL) {
		prev_subtype = type_create(tail->kind, prev_subtype, param_list_create_copy(tail->params));
	}
	
	// Finally, return the final type
	stack_free(s, free);
	return prev_subtype;
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
	p->symbol = symbol_create(SYMBOL_PARAM, type, name);
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
	for (tail = p; tail->next != NULL; tail = tail->next) {
		stack_push(s, (void *) tail);
	}
	
	// Then we pop off elements from the stack and create a new param, keeping this new param
	// to be the next "param" for the next element off the stack
	struct param_list *next_param = NULL;
	while ((tail = (struct param_list *) stack_pop(s)) != NULL) {
		next_param = param_list_create(tail->symbol->name,
									   type_create_copy(tail->symbol->type),
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
	
	scope_bind(p->symbol->name, p->symbol);
	return param_list_resolve(p->next);
}

// Destructor for parameter list
void param_list_free(struct param_list *p) {
	if (p == NULL) return;
	if (p->symbol->type != NULL) type_free(p->symbol->type);
	if (p->next != NULL) 		 param_list_free(p->next);
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
	d->symbol = symbol_create(SYMBOL_UNINITIALIZED, type, name);
	d->value = value;
	d->code = code;
	d->next = next;
	return d;
}

// Name resolving during AST re-traversal for declarations
int decl_resolve(struct decl *d) {
	if (d == NULL) return 0;
	
	int result = 0;
	
	// Update scope (will be either local or global)
	d->symbol->kind = is_global_scope();
	
	// If a variable declaration
	result |= expr_resolve(d->value);
	
	// Add name to current scope (made sure to be done after expr_resolve
	// or else may have had a self-assignment during initialization,
	// i.e. i:integer = i;)
	// But, first check that we haven't defined it already
	if (scope_lookup_current(d->symbol->name) == NULL) { // Not defined
		scope_bind(d->symbol->name, d->symbol);
	} else { // Already defined
		result = 1;
		fprintf(stderr, "ERROR: identifier \"%s\" already declared in this scope\n",
				d->symbol->name);
	}
	
	// If a function declaration, make a new function scope and resolve
	// parameters and code statements
	if (d->code != NULL) {
		scope_enter();
		result |= param_list_resolve(d->symbol->type->params);
		result |= stmt_resolve(d->code);
		scope_exit();
	}
	
	// Chain to the next declaration
	result |= decl_resolve(d->next);
	return result;
}

// Destructor for declarations
void decl_free(struct decl *d) {
	if (d == NULL) return;
	if (d->symbol != NULL) symbol_free(d->symbol);
	if (d->value  != NULL) expr_free(d->value);
	if (d->code   != NULL) stmt_free(d->code);
	if (d->next   != NULL) decl_free(d->next);
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

// Name resolving during AST re-traversal for statements
int stmt_resolve(struct stmt *s) {
	if (s == NULL) return 0;
	
	// Name resolve depending on which type of statement we have
	int result = 0;
	switch (s->kind) {
		case STMT_DECL:
			result |= decl_resolve(s->decl);
			break;
		case STMT_EXPR:
			result |= expr_resolve(s->expr);
			break;
		case STMT_IF_ELSE:
			result |= expr_resolve(s->expr);
			result |= stmt_resolve(s->body);
			result |= stmt_resolve(s->else_body);
			break;
		case STMT_FOR:
			result |= expr_resolve(s->init_expr);
			result |= expr_resolve(s->expr);
			result |= expr_resolve(s->next_expr);
			result |= stmt_resolve(s->body);
			break;
		case STMT_PRINT:
		case STMT_RETURN:
			result |= expr_resolve(s->expr);
			break;
		case STMT_BLOCK:
			scope_enter();
			result |= stmt_resolve(s->body);
			scope_exit();
			break;
		default:
			break;
	}

	// Chain to the next statement
	result |= stmt_resolve(s->next);
	return result;
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
	e->symbol = NULL;
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

// Name resolving during AST re-traversal for expressions (0 for no error, 1 otherwise)
int expr_resolve(struct expr *e) {
	if (e == NULL) return 0;
	
	int result = 0;
	if (e->kind == EXPR_IDENTIFIER) {
		struct symbol *s = scope_lookup(e->name);
		if (s == NULL) {
			result = 1;
			fprintf(stderr, "ERROR: identifier \"%s\" not yet declared\n", e->name);
		}
		e->symbol = scope_lookup(e->name);
	} else {
		result |= expr_resolve(e->left);
		result |= expr_resolve(e->right);
	}
	return result;
}

// Destructor for expressions
void expr_free(struct expr *e) {
	if (e == NULL) return;
	//if (e->kind == EXPR_IDENTIFIER)	    if (e->name != NULL) 		   free(e->name);
	//if (e->kind == EXPR_STRING_LITERAL)	if (e->string_literal != NULL) free(e->string_literal);
	if (e->left  != NULL) expr_free(e->left);
	if (e->right != NULL) expr_free(e->right);
	free(e);
}

// Resolves all names throughout the abstract syntax tree; to be called when AST is assembled
int resolve_names(struct decl *d) {
	GLOBAL_SCOPES = initialize_GLOBAL_SCOPES(); // GLOBAL_SCOPES in scope.h
	
	// Traverse through AST and resolve names
	int result = decl_resolve(d);
	free_GLOBAL_SCOPES();
	
	return result;
}
