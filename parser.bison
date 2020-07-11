%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "stack.h"
#include "visualize_tree.h"

extern int yylex();
extern FILE *yyin;
extern char *yytext;
void yyerror(const char *s);

#include "type.h" // Defines YYSTYPE

struct decl *result;

// Have the declarations and param lists be linked correctly
struct decl 	  *prev_decl;
struct param_list *prev_param;

// Have a stack track the previous statement (push if enters new scope, pop if leaving a scope)
struct stack *prev_stmt_stack;

// Helper functions for maintaining statement stack
void init_stmt_stack() {
	prev_stmt_stack = stack_create();
}
void enter_scope_stmt() {
	stack_push(prev_stmt_stack, NULL);
}
void exit_scope_stmt() {
	stack_pop(prev_stmt_stack);
}
void init_prev_stmt(struct stmt *new_stmt) {
	stack_pop(prev_stmt_stack);
	stack_push(prev_stmt_stack, new_stmt);
}
void link_prev_stmt(struct stmt *new_stmt) {
	struct stmt *prev_stmt = (struct stmt *) stack_pop(prev_stmt_stack);
	prev_stmt->next = new_stmt;
	stack_push(prev_stmt_stack, (void *) new_stmt);
}
void free_stmt_stack() {
	stack_free(prev_stmt_stack, free);
}

%}

%token IDENTIFIER
%token CHAR_LITERAL
%token INTEGER_LITERAL
%token STRING_LITERAL

// Keywords
// Types
%token ARRAY "array" BOOLEAN "boolean" CHAR "char" FUNCTION "function" INTEGER "integer" STRING "string" VOID "void"
// Other
%token ELSE "else" FALSE "false" FOR "for" IF "if" PRINT "print" RETURN "return" TRUE "true" WHILE "while"

// Operators
// Assignment
%token EQUAL '='
// Arithemetic
%token PLUS '+' MINUS '-' MULTIPLY '*' DIVIDE '/' MODULO '%' EXPONENTIATE '^'
// Logical
%token AND "&&" OR "||"
// Comparison
%token LT '<' LT_EQ "<=" GT '>' GT_EQ ">=" IS_EQ "==" IS_NEQ "!="
// Post-increment/decrement
%token POST_INC "++" POST_DEC "--"
// Unary operators
%token NEGATE '!'

// Other
%token LC_PAREN '{' RC_PAREN '}' LR_PAREN '(' RR_PAREN ')' LS_PAREN '[' RS_PAREN ']'
%token COLON ':' SEMICOLON ';' QUOTE '\'' DBQUOTE '\"' COMMA ','

// Operator precendence (lowest to highest)
%left '='
%left "&&" "||"
%left '<' "<=" '>' ">=" "==" "!="
%left '+' '-'
%left '*' '/' '%'
%left '^'
%right '!'
%left "++" "--"
%left '[' '('

// Terminal types
%type <type>		basic_type array_type type return_type arr_ret_type
%type <param_list>	param_list param
%type <decl>		program decls decl variable function
%type <stmt>		func_init comp_statement one_or_more_stmt statement open_statement closed_statement simple_statement	
%type <expr>		basic_var_assign array_assign array_init one_or_more_expr nested_arr_init for_init for_cond for_step expr param_expr zero_or_one_expr
%type <ival>		INTEGER_LITERAL
%type <cval>		CHAR_LITERAL
%type <sval>		IDENTIFIER STRING_LITERAL

%%

program			: { init_stmt_stack(); } decls  { result = $2; free_stmt_stack(); }
				;
decls			: decl							{ $$ = $1; prev_decl = $1; }
				| decls decl					{ $$ = $1; prev_decl->next = $2; prev_decl = $2; }
				;
decl			: variable ';'					{ $$ = $1; }
				| function						{ $$ = $1; }
				;
variable		: IDENTIFIER ':' 
					basic_type
					basic_var_assign			{ $$ = decl_create($1, $3, $4, NULL, NULL); }
				| IDENTIFIER ':'
					"array" 
					'[' INTEGER_LITERAL ']'
					array_type array_assign		{ $$ = decl_create($1, type_create_array($7, $5),
													               $8, NULL, NULL); }
				;
basic_type		: "boolean"						{ $$ = type_create_basic(TYPE_BOOLEAN); }
				| "char"						{ $$ = type_create_basic(TYPE_CHAR); }
				| "integer"						{ $$ = type_create_basic(TYPE_INTEGER); }
				| "string"						{ $$ = type_create_basic(TYPE_STRING); }
				;
basic_var_assign: %empty						{ $$ = NULL; }
				| '=' expr						{ $$ = $2; }
				;
array_type		: type							{ $$ = $1; }
				| "function" return_type
					'(' param_list ')'			{ $$ = type_create_function($2, $4); }
				;
type			: basic_type					{ $$ = $1; }
				| "array"
					'[' INTEGER_LITERAL ']'
					array_type					{ $$ = type_create_array($5, $3); }
				;
return_type		: basic_type					{ $$ = $1; }
				| "void"						{ $$ = type_create_basic(TYPE_VOID); }
				| "array" '[' ']' arr_ret_type	{ $$ = type_create_array($4, -1); }
				| "function" return_type
					'(' param_list ')'			{ $$ = type_create_function($2, $4); }
				;
arr_ret_type	: basic_type					{ $$ = $1; }
				| "array" '[' ']' arr_ret_type	{ $$ = type_create_array($4, -1); }
				| "function" return_type
					'(' param_list ')'			{ $$ = type_create_function($2, $4); }
				;
param_list		: %empty						{ $$ = NULL; }
				| param							{ $$ = $1; prev_param = $1; }
				| param_list ',' param			{ $$ = $1; prev_param->next = $3; prev_param = $3; }
				;
param			: IDENTIFIER ':' return_type	{ $$ = param_list_create($1, $3, NULL); }
				;
array_assign	: %empty						{ $$ = NULL; }
				| '=' '{' array_init '}'		{ $$ = $3; }
				;
array_init		: one_or_more_expr				{ $$ = expr_create(EXPR_ARRAY, $1, NULL); }
				| nested_arr_init				{ $$ = $1; }
				;
one_or_more_expr: expr							{ $$ = expr_create(EXPR_ARG, $1, NULL); }
				| expr ',' one_or_more_expr		{ $$ = expr_create(EXPR_ARG, $1, $3); }
				;
expr			: IDENTIFIER					{ $$ = expr_create_identifier($1); }
				| CHAR_LITERAL					{ $$ = expr_create_char_literal($1); }
				| INTEGER_LITERAL				{ $$ = expr_create_integer_literal($1); }
				| STRING_LITERAL				{ $$ = expr_create_string_literal($1); }
				| "false"						{ $$ = expr_create_boolean_literal(0); }
				| "true"						{ $$ = expr_create_boolean_literal(1); }
				| '(' expr ')'					{ $$ = $2; }
				| expr '[' expr ']'				{ $$ = expr_create(EXPR_SUBSCRIPT, $1, $3); }
				| expr '(' param_expr ')'		{ $$ = expr_create(EXPR_CALL, $1, $3); }
				| expr '=' expr					{ $$ = expr_create(EXPR_ASSIGN, $1, $3); }
				| expr '+' expr					{ $$ = expr_create(EXPR_ADD, $1, $3); }
				| expr '-' expr					{ $$ = expr_create(EXPR_SUB, $1, $3); }
				| expr '*' expr					{ $$ = expr_create(EXPR_MUL, $1, $3); }
				| expr '/' expr					{ $$ = expr_create(EXPR_DIV, $1, $3); }
				| expr '%' expr					{ $$ = expr_create(EXPR_MOD, $1, $3); }
				| expr '^' expr					{ $$ = expr_create(EXPR_EXP, $1, $3); }
				| expr "&&" expr				{ $$ = expr_create(EXPR_AND, $1, $3); }
				| expr "||" expr				{ $$ = expr_create(EXPR_OR, $1, $3); }
				| expr '<' expr					{ $$ = expr_create(EXPR_LT, $1, $3); }
				| expr "<=" expr				{ $$ = expr_create(EXPR_LT_EQ, $1, $3); }
				| expr '>' expr					{ $$ = expr_create(EXPR_GT, $1, $3); }
				| expr ">=" expr				{ $$ = expr_create(EXPR_GT_EQ, $1, $3); }
				| expr "==" expr				{ $$ = expr_create(EXPR_IS_EQ, $1, $3); }
				| expr "!=" expr				{ $$ = expr_create(EXPR_IS_NEQ, $1, $3); }
				| '-' expr			%prec '!'	{ $$ = expr_create(EXPR_SUB,
												    expr_create_integer_literal(0), $2); }
				| '!' expr						{ $$ = expr_create(EXPR_NOT, $2, NULL); }
				| expr "++"						{ $$ = expr_create(EXPR_INC, $1, NULL); }
				| expr "--"						{ $$ = expr_create(EXPR_DEC, $1, NULL); }
				;
param_expr		: %empty						{ $$ = NULL; }
				| one_or_more_expr				{ $$ = $1; }
				;
nested_arr_init : '{' array_init '}'			{ $$ = $2; }
				| nested_arr_init
					',' '{' array_init '}'		{ $$ = expr_create(EXPR_ARG, $1, $4); }
				;
function		: IDENTIFIER ':'
					"function" return_type
					scope_enter
					'(' param_list ')'
					func_init
					/* scope_exit */			{ $$ = decl_create($1, type_create_function($4, $7),
													NULL, $9, NULL);
												  exit_scope_stmt(); }
				;
scope_enter		: %empty						{ enter_scope_stmt(); }
				;
func_init		: ';'							{ $$ = NULL; }
				| '{' comp_statement '}'		{ $$ = stmt_create(STMT_BLOCK, NULL, NULL, NULL,
													NULL, $2, NULL, NULL); }
				;
comp_statement	: %empty						{ $$ = NULL; }
				| one_or_more_stmt				{ $$ = $1; }
				;
one_or_more_stmt: statement						{ $$ = $1; init_prev_stmt($1); }
				| one_or_more_stmt statement	{ $$ = $1; link_prev_stmt($2); }
				;
statement		: open_statement				{ $$ = $1; }
				| closed_statement				{ $$ = $1; }
				;
open_statement	: "if" '(' expr ')' statement	{ $$ = stmt_create(STMT_IF_ELSE, NULL, NULL, $3,
													NULL, $5, NULL, NULL); }
				| "if" '(' expr ')'
					closed_statement
					"else" open_statement		{ $$ = stmt_create(STMT_IF_ELSE, NULL, NULL, $3,
													NULL, $5, $7, NULL); }
				| "while" '(' for_cond ')'
					open_statement				{ $$ = stmt_create(STMT_FOR, NULL, NULL, $3, NULL,
													$5, NULL, NULL); }
				| "for" '(' for_init ';'
					for_cond ';'
					for_step ')'
					open_statement				{ $$ = stmt_create(STMT_FOR, NULL, $3, $5, $7, $9,
													NULL, NULL); }
				;
closed_statement: simple_statement				{ $$ = $1; }
				| "if" '(' expr ')'
					closed_statement
					"else" closed_statement		{ $$ = stmt_create(STMT_IF_ELSE, NULL, NULL, $3,
													NULL, $5, $7, NULL); }
				| "while" '(' for_cond ')'
					closed_statement			{ $$ = stmt_create(STMT_FOR, NULL, NULL, $3, NULL,
													$5, NULL, NULL); }
				| "for" '(' for_init ';'
					for_cond ';'
					for_step ')'
					closed_statement			{ $$ = stmt_create(STMT_FOR, NULL, $3, $5, $7, $9,
													NULL, NULL); }
				; 
simple_statement: decl							{ $$ = stmt_create(STMT_DECL, $1, NULL, NULL, NULL,
													NULL, NULL, NULL); }
				| expr ';'						{ $$ = stmt_create(STMT_EXPR, NULL, NULL, $1, NULL,
													NULL, NULL, NULL); }
				| "print" one_or_more_expr ';'	{ $$ = stmt_create(STMT_PRINT, NULL, NULL, $2, NULL,
													NULL, NULL, NULL); }
				| "return" zero_or_one_expr ';'	{ $$ = stmt_create(STMT_RETURN, NULL, NULL, $2,
													NULL, NULL, NULL, NULL); }
								
				| scope_enter
				  '{' comp_statement '}'
				  /* scope_exit */				{ $$ = stmt_create(STMT_BLOCK, NULL, NULL, NULL,
													NULL, $3, NULL, NULL);
												  exit_scope_stmt(); }
				;
for_init		: %empty						{ $$ = NULL; }
				| one_or_more_expr				{ $$ = $1; }
				;
for_cond		: %empty						{ $$ = NULL; }
				| expr							{ $$ = $1; }
				;
for_step		: %empty						{ $$ = NULL; }
				| one_or_more_expr				{ $$ = $1; }
				;
zero_or_one_expr: %empty						{ $$ = NULL; }
				| expr							{ $$ = $1; }
				;

%%

void yyerror(const char *s)
{
  extern int yylineno;	// Defined and maintained in scanner.c
  extern char *yytext;	// Defined and maintained in scanner.c
  
  fprintf(stderr, "ERROR: %s at symbol \"%s\" on line %d\n", s, yytext, yylineno);
}

#define PRINT_WIDTH 70

// Crude function to print a string, followed by dashes, followed by "PASS" or "FAIL".
// Assumes that user will have a str less than the maximum width.
void pretty_print(const char *s) {
	int num_dashes = PRINT_WIDTH - strlen(s) - 6;
	if (num_dashes < 0) { // String is too long
		printf("%s ", s);
	} else {			  // Pad with dashes
		char *dashes = malloc(sizeof(char) * (num_dashes + 1));
		if (dashes != NULL) {
			for (int i = 0; i < num_dashes; i++)
				dashes[i] = '-';
			dashes[num_dashes] = '\0';
			printf("%s %s ", s, dashes);
			free(dashes);
		} else { // Malloc failed, do whatever
			printf("%s ", s);
		}
	}
}

int main(int argc, char *argv[]) {
	char *inputFile;
	
	if (argc > 1) inputFile = argv[1];
	
	yyin = fopen(inputFile, "r");
	if (!yyin) {
		fprintf(stderr, "Could not open: %s\n", inputFile);
		return 1;
	}
	
	// Parse the program
	pretty_print("Parsing the program");
	if (yyparse() == 0) { 	// If successful
		printf("PASS\n");
	
		// Print out AST in SVG for visualization
		char *svgFile = malloc(strlen(inputFile) + 5);
		sprintf(svgFile, "%s.svg", inputFile);
		
		visualize_tree(result, svgFile);
		
		free(svgFile);
	} else {				// If unsuccessful
		printf("FAIL\n");
		fprintf(stderr, "Parse unsuccessful :c\n");
		exit(1);
	}
	fclose(yyin);
	
	// Perform name resolution and type checking
	pretty_print("Performing semantic analysis");
	if (perform_semantic_analysis(result) == 0) { // If successful
		printf("PASS\n");
	} else {						 			  // If unsuccessful
		printf("FAIL\n");
		fprintf(stderr, "Semantic analysis failed :c\n");
		exit(1);
	}
	
	printf("Front end successful c:\n");
	decl_free(result); // Free up AST
	return 0;
}
