#ifndef AST_H
#define AST_H

#include "ast_type.h"

// Declarations
struct decl {
	const char *name;			// Identifier
	struct type *type;			// Its Type
	struct expr *value;			// Initialization if declared
	struct stmt *code;			// Code for functions
	struct decl *next;			// Next declaration (if there is one)
};

// Constructor for declarations
struct decl *decl_create(const char *name, struct type *type, struct expr *value,
	struct stmt *code, struct decl *next);

// Performs name resolution and typechecking on the declarations in the AST
void decl_semantic_check(struct decl *d, int *error_count);

// Destructor for declarations
void decl_free(struct decl *d);

// Statements
typedef enum {
	STMT_DECL,					// Local declaration statement
	STMT_EXPR,					// Expression statement
	STMT_IF_ELSE,				// If (+else) statement
	STMT_FOR,					// For statement
	STMT_PRINT,					// Print statement
	STMT_RETURN,				// Return statement
	STMT_BLOCK					// Multiple statements enclosed in curly braces
} stmt_t;

struct stmt {
	stmt_t kind;
	struct decl *decl;			// Local declaration statement
	struct expr *init_expr;		// For initialization
	struct expr *expr;			// Conditional expression (i.e. if, while, for);
								// Also arguments for print and return
	struct expr *next_expr;		// For step
	struct stmt *body;			// Executed if condition (i.e. if, while, for);
								// Also contents of a block statement
	struct stmt *else_body;		// Else body for if statement
	struct stmt *next;			// Next statement
};

// Constructor for statements
struct stmt *stmt_create(stmt_t kind, struct decl *decl, struct expr *init_expr,
	struct expr *expr, struct expr *next_expr, struct stmt *body,
	struct stmt *else_body, struct stmt *next);

// Performs name resolution and typechecking on the statements in the AST. Provide
// the name and return type of the function as to type check whether the function
// actually returns that type
void stmt_semantic_check(struct stmt *s, const char* name, struct type *return_type,
						 int *error_count);

// Destructor for statements
void stmt_free(struct stmt *s);

// Expressions
typedef enum {
	EXPR_IDENTIFIER,
	EXPR_INTEGER_LITERAL,
	EXPR_CHAR_LITERAL,
	EXPR_STRING_LITERAL,
	EXPR_BOOLEAN_LITERAL,
	EXPR_ARRAY,
	EXPR_SUBSCRIPT,
	EXPR_CALL,
	EXPR_ARG,
	EXPR_ASSIGN,
	EXPR_ADD,
	EXPR_SUB,
	EXPR_MUL,
	EXPR_DIV,
	EXPR_MOD,
	EXPR_EXP,
	EXPR_AND,
	EXPR_OR,
	EXPR_LT,
	EXPR_LT_EQ,
	EXPR_GT,
	EXPR_GT_EQ,
	EXPR_IS_EQ,
	EXPR_IS_NEQ,
	EXPR_NOT,
	EXPR_INC,
	EXPR_DEC
} expr_t;

struct expr {
	expr_t kind;					// Type of expression
	struct expr *left;				// Used if an expression is an operator
	struct expr *right;				// Used if an expression is an operator
	union {
		const char *name;			// Used if an identifier
		int integer_value;			// Used if an integer literal, boolean, or character literal
		const char *string_literal;	// Used if a string literal
	};
};

// Constructor for expressions
struct expr *expr_create(expr_t kind, struct expr *left, struct expr *right);

// Factory constructors for expressions
struct expr *expr_create_identifier(const char *name);
struct expr *expr_create_integer_literal(int i);
struct expr *expr_create_char_literal(char c);
struct expr *expr_create_string_literal(const char *str);
struct expr *expr_create_boolean_literal(int b);

// Copy constructor for expressions
struct expr *expr_create_copy(struct expr *e);

// Performs name resolution and typechecking on the expressions in the AST. Returns
// the "expected" type (especially if there's conflicting types, an error message 
// will be emitted and the expected type will be returned)
struct type *expr_semantic_check(struct expr *e, int *error_count);

// Destructor for expressions
void expr_free(struct expr *e);

/*
 * ===== WHEN DONE PERFORMING AST CONSTRUCTION =================================
 */

// Performs name resolution and type checking on AST. Returns number of errors.
int perform_semantic_analysis(struct decl *ast);

// Custom printf family for printing types, expressions, etc.
int ast_vfprintf(FILE *stream, const char *fmt, va_list args);
int ast_vprintf(const char *fmt, va_list args);
int ast_fprintf(FILE *stream, const char *fmt, ...);
int ast_printf(const char *fmt, ...);

#endif /* AST_H */

