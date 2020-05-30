#ifndef DECL_H
#define DECL_H

// Atomic types
typedef enum {
	TYPE_VOID,
	TYPE_BOOLEAN,
	TYPE_CHAR,
	TYPE_INTEGER,
	TYPE_STRING,
	TYPE_ARRAY,
	TYPE_FUNCTION
} type_t;

struct type {
	type_t kind;
	struct type *subtype;		// For arrays, return type for functions
	struct param_list *params;	// For functions
};

// Constructor for type
struct type *type_create(type_t kind, struct type *subtype,
	struct param_list *params);

// Destructor for type
void type_free(struct type *t);

// Parameter list for functions
struct param_list {
	const char *name;			// Identifier
	struct type *type;			// Type
	struct param_list *next;	// Next param in list
};

// Constructor for parameter list
struct param_list *param_list_create(const char *name, struct type *type,
	struct param_list *next);

// Destructor for parameter list
void param_list_free(struct param_list *p);

// Declarations
struct decl {
	const char *name;			// Identifier
	struct type *type;			// Type
	struct expr *value;			// Initialization if declared
	struct stmt *code;			// Code for functions
	struct decl *next;			// Next declaration (if there is one)
};

// Constructor for declarations
struct decl *decl_create(const char *name, struct type *type, struct expr *value,
	struct stmt *code, struct decl *next);
	
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
	struct expr *expr;			// Conditional expression (i.e. if, while, for)
	struct expr *next_expr;		// For step
	struct stmt *body;			// Executed if condition (i.e. if, while, for)
	struct stmt *else_body;		// Else body for if statement
	struct stmt *next;			// Next statement
};

// Constructor for statements
struct stmt *stmt_create(stmt_t kind, struct decl *decl, struct expr *init_expr,
	struct expr *expr, struct expr *next_expr, struct stmt *body,
	struct stmt *else_body, struct stmt *next);
	
// Destructor for statements
void stmt_free(struct stmt *s);

// Expressions
typedef enum {
	EXPR_IDENTIFIER,
	EXPR_INTEGER_LITERAL,
	EXPR_CHAR_LITERAL,
	EXPR_STRING_LITERAL,
	EXPR_BOOLEAN_LITERAL,
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
	expr_t kind;
	struct expr *left;
	struct expr *right;
	const char *name;
	int integer_value;
	const char *string_literal;
};

// Constructor for expressions
struct expr *expr_create(expr_t kind, struct expr *left, struct expr *right);

// Factory constructors for expressions
struct expr *expr_create_identifier(const char *name);
struct expr *expr_create_integer_literal(int i);
struct expr *expr_create_char_literal(char c);
struct expr *expr_create_string_literal(const char *str);
struct expr *expr_create_boolean_literal(int b);

// Destructor for expressions
void expr_free(struct expr *e);

#endif /* DECL_H */

