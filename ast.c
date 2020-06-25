#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "scope.h"
#include "stack.h"
#include "symbol.h"

// If a decl is declared but not initialized, this function will provide a
// default initialization (basically either 0 or NULL depending on the type)
struct expr *_decl_default_initialization(struct type *t) {
	struct expr *result, *temp, *next;
	
	switch (t->kind) {
		case TYPE_VOID: // For function return types
			return NULL;
		case TYPE_BOOLEAN:
			return expr_create_boolean_literal(0);
		case TYPE_CHAR:
			return expr_create_char_literal(0);
		case TYPE_INTEGER:
			return expr_create_integer_literal(0);
		case TYPE_STRING:
			return expr_create_string_literal(NULL);
		case TYPE_ARRAY:
			// We will initialize based on its subtype
			// First, create a dummy expr that represents an element of the
			// array that we will repeatedly copy from
			temp = _decl_default_initialization(t->subtype);
			temp = expr_create(EXPR_ARG, temp, NULL);
			
			// Secondly, make copies as needed and fill up the array
			result = NULL;
			for (int i = 0; i < t->num_elems; i++) {
				if (i == 0) {
					result = expr_create_copy(temp);
					next = result;
				} else {
					next->right = expr_create_copy(temp);
					next = next->right;
				}
			}
			expr_free(temp);
			return result;
		case TYPE_FUNCTION:
			// For now, according to my knowledge, a function is placed in an
			// address in memory. When we update an array of functions, we are
			// actually just updating the address of the function. Thus, since
			// we don't know which address the user wants, set the address to
			// the default NULL.
			// However, I do note that if we were to store a function into an
			// array, we'd be leaving its identifier, but alas, the NULL would
			// be overwritten and a lookup for identifier NULL should result in
			// the NULL address returned...
			return NULL;
		default:
			return NULL;
	}
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
	if (value == NULL) {
		d->value = _decl_default_initialization(type);
	} else {
		d->value = value;
	}
	d->code = code;
	d->next = next;
	return d;
}

// Name resolving during AST re-traversal for declarations
int decl_resolve(struct decl *d) {
	if (d == NULL) return 0;
	
	int result = 0;
	
	// If a variable declaration
	result |= expr_resolve(d->value);
	
	// Add name to current scope (made sure to be done after expr_resolve
	// or else may have had a self-assignment during initialization,
	// i.e. i:integer = i;)
	// But, first check that we haven't defined it already
	if (scope_lookup_current(d->name) == NULL) { // Not defined
		scope_bind(d->name, symbol_create(is_global_scope(), d->name, d->type));
	} else { // Already defined
		result = 1;
		fprintf(stderr, "ERROR: identifier \"%s\" already declared in this scope\n",
				d->name);
	}
	
	// If a function declaration, make a new function scope and resolve
	// parameters and code statements
	if (d->code != NULL) {
		scope_enter();
		result |= param_list_resolve(d->type->params);
		result |= stmt_resolve(d->code);
		scope_exit();
	}
	
	// Chain to the next declaration
	result |= decl_resolve(d->next);
	return result;
}

// Performs typechecking on the declarations in the AST
void decl_typecheck(struct decl *d, int *error_count) {
	if (d == NULL) return;
	
	// Check if initialization type is same a declared type
	if (d->value != NULL) {
		struct type *t = expr_typecheck(d->value, error_count);
		if (!type_equals(t, d->type)) {
			(*error_count)++;
			ast_fprintf(stderr, "ERROR: Incorrect type <%T> used for identifier %s of type <%T>\n",
						t, d->name, d->type);
		}
		type_free(t);
	}
	
	// If we have a function, check the types within the code statements
	if (d->type->kind == TYPE_FUNCTION && d->code != NULL) {
		stmt_typecheck(d->code, d->name, d->type->subtype, error_count);
	}
}

// Destructor for declarations
void decl_free(struct decl *d) {
	if (d == NULL) return;
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

// Performs typechecking on the statements in the AST. Provide the name and return type
// of the function as to type check whether the function actually returns that type
void stmt_typecheck(struct stmt *s, 
					const char* name,
					struct type *return_type,
					int *error_count)
{
	if (s == NULL) return;
	
	struct type *t;
	switch(s->kind) {
		case STMT_DECL:
			decl_typecheck(s->decl, error_count);
			break;
		case STMT_EXPR:
			t = expr_typecheck(s->expr, error_count);
			type_free(t);
			break;
		case STMT_IF_ELSE:
			t = expr_typecheck(s->expr, error_count);
			if (t->kind != TYPE_BOOLEAN) {
				(*error_count)++;
				ast_fprintf(stderr, "ERROR: In function %s, in IF statement, "
									"expected type <boolean>, got type <%T>\n",
							name, t);
			}
			type_free(t);
			stmt_typecheck(s->body, name, return_type, error_count);
			stmt_typecheck(s->else_body, name, return_type, error_count);
			break;
		case STMT_FOR:
			// Only have to check the condition part. The initialization and step part
			// are usually assignments, but can do whatever they want (i.e. function
			// calls with side effects)
			t = expr_typecheck(s->expr, error_count);
			if (t != NULL && t->kind != TYPE_BOOLEAN) {
				(*error_count)++;
				ast_fprintf(stderr, "ERROR: In function %s, in FOR statement condition, "
									"expected type <boolean>, got type <%T>\n",
							name, t);
			}
			type_free(t);
			stmt_typecheck(s->body, name, return_type, error_count);
			break;
		case STMT_PRINT:
			// Apparently, the language specification says we can print any type.
			// Good luck future me <--- >:c
			// So for now, just check the types are fine internally for each print
			// argument
			t = expr_typecheck(s->expr, error_count);
			type_free(t);
			break;
		case STMT_RETURN:
			// Ensure correct return type of a function
			t = expr_typecheck(s->expr, error_count);
			if (t == NULL) {
				if (return_type->kind != TYPE_VOID) {
					// User wrote a "return;" but their function doesn't return "void"
					(*error_count)++;
					ast_fprintf(stderr, "ERROR: In function %s, returning type <void> in a "
										"function that returns <%T>\n",
								name, return_type);
				}
			} else if (!type_equals(t, return_type)) {
				(*error_count)++;
				ast_fprintf(stderr, "ERROR: In function %s, returning type <%T> in a "
									"function  that returns <%T>\n",
							name, t, return_type);
			}
			type_free(t);
			break;
		case STMT_BLOCK:
			stmt_typecheck(s->body, name, return_type, error_count);
			break;
		default:
			break;
	}
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

// Copy constructor for expressions
struct expr *expr_create_copy(struct expr *e) {
	if (e == NULL) return NULL;

	struct expr *new = malloc(sizeof(struct expr));
	
	new->kind = e->kind;
	new->left = expr_create_copy(e->left);
	new->right = expr_create_copy(e->right);
	switch (e->kind) {
		case EXPR_IDENTIFIER:
			new->name = e->name;
			break;
		case EXPR_INTEGER_LITERAL:
		case EXPR_CHAR_LITERAL:
		case EXPR_BOOLEAN_LITERAL:
			new->integer_value = e->integer_value;
			break;
		case EXPR_STRING_LITERAL:
			new->string_literal = e->string_literal;
			break;
		default:
			break;
	}
	return new;
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
		//e = scope_lookup(e->name);
	} else {
		result |= expr_resolve(e->left);
		result |= expr_resolve(e->right);
	}
	return result;
}

// Performs typechecking on the expressions in the AST. Returns the "expected"
// type (especially if there's conflicting types, an error message will be emitted
// and the expected type will be returned)
struct type *expr_typecheck(struct expr *e, int *error_count) {
	if (e == NULL) return NULL;
	
	struct type *left = expr_typecheck(e->left, error_count);
	struct type *right = expr_typecheck(e->right, error_count);

	struct param_list *expected;
	struct expr *given;
	int flag = 0, params_expected = 0, params_given = 0;
	char *operation;
	struct type *result;
	switch (e->kind) {
		case EXPR_IDENTIFIER:
			result = type_create_copy(scope_lookup(e->name)->type);
			break;
		case EXPR_INTEGER_LITERAL:
			result = type_create_basic(TYPE_INTEGER);
			break;
		case EXPR_CHAR_LITERAL:
			result = type_create_basic(TYPE_CHAR);
			break;
		case EXPR_STRING_LITERAL:
			result = type_create_basic(TYPE_STRING);
			break;
		case EXPR_BOOLEAN_LITERAL:
			result = type_create_basic(TYPE_BOOLEAN);
			break;
		case EXPR_SUBSCRIPT:
			if (left->kind == TYPE_ARRAY) {
				if (right->kind != TYPE_INTEGER) {
					(*error_count)++;
					ast_fprintf(stderr, "ERROR: Array index is not an integer, instead is: <%T>\n",
								right);
				}
				result = type_create_copy(left->subtype);
			} else {
				(*error_count)++;
				ast_fprintf(stderr, "ERROR: Array type expected, got: <%T>\n", left);
				// Isn't an array, but we need to return some type, so just return it
				result = type_create_copy(left);
			}
			break;
		case EXPR_CALL:
			// The function signature is stored in "left"
			// We must check if "left" was an function or not
			if (left->kind == TYPE_FUNCTION) {
			
				// Check if we were given the correct number of parameters
				params_expected = params_given = 0;
				for (expected = left->params; expected != NULL; expected = expected->next)
					params_expected++;
				// For traversing the right side of the expression tree, it will be a
				// linked list of EXPR_ARG
				for (given = e->right; given != NULL; given = given->right)
					params_given++;
				if (params_expected != params_given) {
					(*error_count)++;
					fprintf(stderr, "ERROR: In function call, incorrect number of parameters "
									"passed. Expected %d, got %d\n",
							params_expected, params_given);
				} else {
		
					// Now we check if the types of params provided are the same
					// as the ones expected by the function
					params_expected = 1; // Parameter index counter
					for (expected = left->params, given = e->right;
							expected != NULL && given != NULL;
							expected = expected->next, given = given->right, params_expected++) {
						if (!type_equals(expected->type, expr_typecheck(given, error_count))) {
							(*error_count)++;
							ast_fprintf(stderr, "ERROR: In function call, for parameter #%d, "
											    "expected type <%T>, got <%T>\n",
									    params_expected, expected, given);
						}
					}
				}
				result = type_create_copy(left->subtype);
			} else { // Not a function on LHS
				(*error_count)++;
				ast_fprintf(stderr, "ERROR: Function type expected, got: <%T>\n", left);
				// Isn't a function, but we need to return some type, so just return it
				result = type_create_copy(left);
			}
			break;
		case EXPR_ARG:
			// Type lives in the left node, the right node is the rest of the linked list
			result = type_create_copy(left);
			break;
		case EXPR_ASSIGN:
			if (!type_equals(left, right)) {
				(*error_count)++;
				ast_fprintf(stderr, "ERROR: Assigning of two incompatible types: <%T> and <%T>\n",
						    left, right);
			}
			// Have to return something, so keep it right associative
			result = type_create_copy(right);
			break;
		case EXPR_ADD:
			operation = "Adding";
			goto arithemetic_operator; // Heh what are you gonna do about it?
		case EXPR_SUB:
			operation = "Subtracting";
			goto arithemetic_operator; // I'm too lazy to make another helper function
		case EXPR_MUL:
			operation = "Multiplying";
			goto arithemetic_operator;
		case EXPR_DIV:
			operation = "Dividing";
			goto arithemetic_operator;
		case EXPR_MOD:
			operation = "Modulo of";
			goto arithemetic_operator;
		case EXPR_EXP:
			operation = "Exponentiation of";
arithemetic_operator:
			// Need to check if both sides are integers in order to do arithemetic
			if (left->kind != TYPE_INTEGER || right->kind != TYPE_INTEGER) {
				(*error_count)++;
				ast_fprintf(stderr, "ERROR: %s two incompatible types: <%T> and <%T>\n",
						    operation, left, right);
			}
			// Return an integer type as the expected type
			result = type_create_basic(TYPE_INTEGER);
			break;
		case EXPR_AND:
			operation = "Logical AND of";
			goto boolean_comparison;
		case EXPR_OR:
			operation = "Logical OR of";
			goto boolean_comparison;
		case EXPR_LT:
			operation = "Less than comparison of";
			goto boolean_comparison;
		case EXPR_LT_EQ:
			operation = "Less than or equal to comparison of";
			goto boolean_comparison;
		case EXPR_GT:
			operation = "Greater than comparison of";
			goto boolean_comparison;
		case EXPR_GT_EQ:
			operation = "Greater than or equal to comparison of";
			goto boolean_comparison;
		case EXPR_IS_EQ:
			operation = "Equality comparison of";
			goto boolean_comparison;
		case EXPR_IS_NEQ:
			operation = "Inequality comparison of";
boolean_comparison:
			// First, check if LHS and RHS have valid types for boolean comparisons
			if (left->kind == TYPE_VOID
					|| left->kind == TYPE_ARRAY
					|| left->kind == TYPE_FUNCTION) {
				(*error_count)++;
				ast_fprintf(stderr,
						    "ERROR: %s an invalid type for comparison on left hand side: <%T>\n",
						    operation, left);
				flag = 1;
			}
			if (right->kind == TYPE_VOID
					|| right->kind == TYPE_ARRAY
					|| right->kind == TYPE_FUNCTION) {
				(*error_count)++;
				ast_fprintf(stderr,
						    "ERROR: %s an invalid type for comparison on right hand side: <%T>\n",
						     operation, right);
				flag = 1;
			}
			// If LHS and RHS are valid types, check their type equality
			if (!flag && !type_equals(left, right)) {
				(*error_count)++;
				ast_fprintf(stderr, "ERROR: %s two incompatible types: <%T> and <%T>\n",
						    operation, left, right);
			}
			// Return a boolean type as the expected type
			result = type_create_basic(TYPE_BOOLEAN);
			break;
		case EXPR_NOT:
			operation = "Logical NOT of";
			if (left->kind != TYPE_BOOLEAN) {
				(*error_count)++;
				ast_fprintf(stderr, "ERROR: %s an invalid type: <%T>\n",
						    operation, left);
			}
			result = type_create_basic(TYPE_BOOLEAN);
			break;
		case EXPR_INC:
			operation = "Post-increment of";
			if (left->kind != TYPE_INTEGER) {
				(*error_count)++;
				ast_fprintf(stderr, "ERROR: %s an invalid type: <%T>\n",
						    operation, left);
			}
			result = type_create_basic(TYPE_INTEGER);
			break;
		case EXPR_DEC:
			operation = "Post-decrement of";
			if (left->kind != TYPE_INTEGER) {
				(*error_count)++;
				ast_fprintf(stderr, "ERROR: %s an invalid type: <%T>\n",
						    operation, left);
			}
			result = type_create_basic(TYPE_INTEGER);
			break;
		default:
			result = NULL;
			break;
	}
	
	type_free(left);
	type_free(right);
	puts("Done type checking expr");
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

// Performs name resolution and type checking on AST. Returns number of errors.
int perform_semantic_analysis(struct decl *ast) {
	int num_errors;

	GLOBAL_SCOPES = initialize_GLOBAL_SCOPES(); // GLOBAL_SCOPES in scope.h
	
	// Perform name resolution
	num_errors = decl_resolve(ast);
	free_GLOBAL_SCOPES();
	
	// Perform type checking
	//decl_typecheck(ast, &num_errors);
	
	// Perform array initialization checking
	// TODO
	
	return num_errors;
}

#define AST_PRINTF_BUFFER 256

// Custom printf family for printing types, expressions, etc.
int ast_vfprintf(FILE *stream, const char *fmt, va_list args) {
	if (fmt == NULL) return -1;

	char buffer[AST_PRINTF_BUFFER];
	int i = 0, total = 0;
	
	for (const char *c = fmt; *c != '\0'; c++) {
		// Skip every character that isn't a '%'
		while (*c != '%' && *c != '\0') {
			buffer[i] = *c;
			i++;
			if (i == (AST_PRINTF_BUFFER - 1)) { // Max buffer capacity
				buffer[i] = '\0';				// Print to screen
				fprintf(stream, "%s", buffer);
				total += AST_PRINTF_BUFFER - 1;	// Store total
				i = 0;							// Reset
			}
			c++;
		}
		if (*c == '\0') break;
	
		// We have a '%'
		if (*c == '%') {
			c++; // Advance past the '%'
			
			// Figure out which '%?' type we have
			char *result = NULL;
			union {
				char *s;
				struct {
					int d;
					int length;
				};
			} temp; 
			switch (*c) {
				case 'E': // Expression
					break;
				case 'T': // Type
					result = type_stringify(va_arg(args, struct type *));
					break;
				case 'd': case 'i': // Decimal
					temp.d = va_arg(args, int);
					temp.length = snprintf(NULL, 0, "%d", temp.d);
					result = malloc(temp.length + 1);
					snprintf(result, temp.length + 1, "%d", temp.d);
					break;
				case 's': // String
					temp.s = va_arg(args, char *);
					result = malloc(strlen(temp.s));
					strcpy(result, temp.s);
					break;
				default:
					break;
			}
			
			// See if we've overflown the buffer
			int length = strlen(result);
			int delta = i + length - (AST_PRINTF_BUFFER - 1);
			
			if (delta >= 0) { // Overflown, break it up into 2 parts
				// We've overflown by length - delta
				strncpy(buffer + i, result, length - delta);
				buffer[AST_PRINTF_BUFFER-1] = '\0';
				fprintf(stream, "%s", buffer);
				total += AST_PRINTF_BUFFER - 1;
				strncpy(buffer, result + (length - delta), delta);
				i = delta;
			} else { // Not overflown, copy to buffer
				strncpy(buffer + i, result, length);
				i += length;
			}
			
			if (result != NULL) free(result);
		}
	}

	// Flush out rest of buffer
	if ((total + i) > 0) {
		fprintf(stream, "%.*s", i, buffer);
	}	
	return total + i;
}

int ast_vprintf(const char *fmt, va_list args) {
	return ast_vfprintf(stdout, fmt, args);
}

int ast_fprintf(FILE *stream, const char *fmt, ...) {
	int result;

	va_list args;
	va_start(args, fmt);
	result = ast_vfprintf(stream, fmt, args);
	va_end(args);
	
	return result;
}

int ast_printf(const char *fmt, ...) {
	int result;

	va_list args;
	va_start(args, fmt);
	result = ast_vprintf(fmt, args);
	va_end(args);
	
	return result;
}
