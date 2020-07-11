#ifndef AST_TYPE_H
#define AST_TYPE_H

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
	struct type *subtype;			// For arrays, return type for functions
	union {
		int num_elems;				// Number of elements for arrays
		struct param_list *params;	// Parameters for functions
	};
};

// Constructor for type
struct type *type_create_basic(type_t kind);
struct type *type_create_array(struct type *subtype, int num_elems);
struct type *type_create_function(struct type *subtype, struct param_list *params);

// Copy constructor for type
struct type *type_create_copy(struct type *t);

// Returns 0 if not equal, 1 if equal. Does not check if array bounds are equal
int type_equals(struct type *a, struct type *b);

// Used for checking two array types are of the same array sizes.
// Returns 0 if not equal, 1 if equal, -1 if at least one is not an array.
int type_equals_array_bounds(struct type *a, struct type *b);

// Returns what type 't' is in a newly allocated C string
char *type_stringify(struct type *t);

// Destructor for type
void type_free(struct type *t);

// Parameter list for functions
struct param_list {
	const char *name;			// Identifier
	struct type *type;			// Its type
	struct param_list *next;	// Next param in list
};

// Constructor for parameter list
struct param_list *param_list_create(const char *name, struct type *type,
	struct param_list *next);

// Copy constructor for parameter list
struct param_list *param_list_create_copy(struct param_list *p);

// Name resolving during AST re-traversal for parameter lists
// i.e. (have identifiers available for function code)
int param_list_resolve(struct param_list *p);

// Destructor for parameter list
void param_list_free(struct param_list *p);

#endif /* AST_TYPE_H */

