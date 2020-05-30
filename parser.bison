%{

#include <stdio.h>
#include <stdlib.h>
#include "decl.h"

extern int yylex();
extern FILE *yyin;
extern char *yytext;
void yyerror(const char *s);

#include "type.h" // Defines YYSTYPE

struct decl *result;

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

// Operator precendence
%left '[' '('
%left "++" "--"
%right '!'
%left '^'
%left '*' '/' '%'
%left '+' '-'
%left '<' "<=" '>' ">=" "==" "!="
%left "&&" "||"
%left '=' 

// Terminal types
%type <type>		basic_type array_type type return_type arr_ret_type
%type <param_list>	param_list param
%type <decl>		program decls decl variable function
%type <stmt>		func_init func_body statement open_statement closed_statement simple_statement	comp_statement
%type <expr>		basic_var_assign array_assign array_init for_init for_cond for_step print_body expr param_expr
%type <ival>		INTEGER_LITERAL
%type <cval>		CHAR_LITERAL
%type <sval>		IDENTIFIER STRING_LITERAL

%%

program			: decls							{ result = $1; }
				;
decls			: decls decl					{ $$ = $1; ($1)->next = $2; }
				| decl							{ $$ = $1; }
				;
decl			: variable ';'					{ $$ = $1; }
				| function						{ $$ = $1; }
				;
variable		: IDENTIFIER ':' basic_type basic_var_assign
												{ $$ = decl_create($1, $3, $4, NULL, NULL); }
				| IDENTIFIER ':' "array" '[' INTEGER_LITERAL ']' array_type array_assign
												{ $$ = decl_create($1,
													type_create(TYPE_ARRAY, $7, NULL),
													$8, NULL, NULL); }
				;
basic_type		: "boolean"						{ $$ = type_create(TYPE_BOOLEAN, NULL, NULL); }
				| "char"						{ $$ = type_create(TYPE_CHAR, NULL, NULL); }
				| "integer"						{ $$ = type_create(TYPE_INTEGER, NULL, NULL); }
				| "string"						{ $$ = type_create(TYPE_STRING, NULL, NULL); }
				;
basic_var_assign: %empty						{ $$ = NULL; }
				| '=' expr						{ $$ = $2; }
				;
array_type		: type							{ $$ = $1; }
				| "function" return_type '(' param_list ')'
												{ $$ = type_create(TYPE_FUNCTION, $2, $4); }
				;
type			: basic_type					{ $$ = $1; }
				| "array" '[' INTEGER_LITERAL ']' array_type
												{ $$ = type_create(TYPE_ARRAY, $5, NULL); }
				;
return_type		: basic_type					{ $$ = $1; }
				| "void"						{ $$ = type_create(TYPE_VOID, NULL, NULL); }
				| "array" '[' ']' arr_ret_type	{ $$ = type_create(TYPE_ARRAY, $4, NULL); }
				| "function" return_type '(' param_list ')'
												{ $$ = type_create(TYPE_FUNCTION, $2, $4); }
				;
arr_ret_type	: basic_type					{ $$ = $1; }
				| "array" '[' ']' arr_ret_type	{ $$ = type_create(TYPE_ARRAY, $4, NULL); }
				| "function" return_type '(' param_list ')'
												{ $$ = type_create(TYPE_FUNCTION, $2, $4); }
				;
param_list		: %empty						{ $$ = NULL; }
				| param							{ $$ = $1; }
				| param_list ',' param			{ $$ = $1; ($1)->next = $3; }
				;
param			: IDENTIFIER ':' return_type	{ $$ = param_list_create($1, $3, NULL); }
				;
array_assign	: %empty						{ $$ = NULL; }
				| '=' '{' array_init '}'		{ $$ = $3; }
				;
array_init		: expr							{ $$ = $1; }
				| '{' array_init '}'			{ $$ = $2; }
				| array_init ',' expr			{ $$ = expr_create(EXPR_ARG, $3, $1); }
				;
function		: IDENTIFIER ':' "function" return_type '(' param_list ')' func_init
												{ $$ = decl_create($1,
													type_create(TYPE_FUNCTION, $4, $6),
													NULL, $8, NULL); }
				;
func_init		: ';'							{ $$ = NULL; }
				| '{' func_body '}'				{ $$ = $2; }
				;
func_body		: comp_statement				{ $$ = $1; }
				| %empty						{ $$ = NULL; }
				;
comp_statement	: statement						{ $$ = $1; }
				| comp_statement statement		{ $$ = stmt_create(STMT_BLOCK, NULL, NULL, NULL,
													NULL, $1, NULL, $2); }
				;
statement		: open_statement				{ $$ = $1; }
				| closed_statement				{ $$ = $1; }
				;
open_statement	: "if" '(' expr ')' statement	{ $$ = stmt_create(STMT_IF_ELSE, NULL, NULL, $3,
													NULL, $5, NULL, NULL); }
				| "if" '(' expr ')' closed_statement "else" open_statement
												{ $$ = stmt_create(STMT_IF_ELSE, NULL, NULL, $3,
													NULL, $5, $7, NULL); }
				| "while" '(' expr ')' open_statement
												{ $$ = stmt_create(STMT_FOR, NULL, NULL, $3, NULL,
													$5, NULL, NULL); }
				| "for" '(' for_init ';' for_cond ';' for_step ')' open_statement
												{ $$ = stmt_create(STMT_FOR, NULL, $3, $5, $7, $9,
													NULL, NULL); }
				;
closed_statement: simple_statement
												{ $$ = $1; }
				| "if" '(' expr ')' closed_statement "else" closed_statement
												{ $$ = stmt_create(STMT_IF_ELSE, NULL, NULL, $3,
													NULL, $5, $7, NULL); }
				| "while" '(' expr ')' closed_statement
												{ $$ = stmt_create(STMT_FOR, NULL, NULL, $3, NULL,
													$5, NULL, NULL); }
				| "for" '(' for_init ';' for_cond ';' for_step ')' closed_statement
												{ $$ = stmt_create(STMT_FOR, NULL, $3, $5, $7, $9,
													NULL, NULL); }
				; 
simple_statement: decl							{ $$ = stmt_create(STMT_DECL, $1, NULL, NULL, NULL,
													NULL, NULL, NULL); }
				| expr ';'						{ $$ = stmt_create(STMT_EXPR, NULL, NULL, $1, NULL,
													NULL, NULL, NULL); }
				| "print" print_body ';'		{ $$ = stmt_create(STMT_PRINT, NULL, NULL, $2, NULL,
													NULL, NULL, NULL); }
				| "return" expr ';'				{ $$ = stmt_create(STMT_RETURN, NULL, NULL, $2,
													NULL, NULL, NULL, NULL); }
				| '{' '}'						{ $$ = NULL; }
				| '{' comp_statement '}'		{ $$ = $2; }
				;
for_init		: %empty						{ $$ = NULL; }
				| expr							{ $$ = expr_create(EXPR_ARG, $1, NULL); }
				| for_init ',' expr				{ $$ = expr_create(EXPR_ARG, $3, $1); }
				;
for_cond		: %empty						{ $$ = NULL; }
				| expr							{ $$ = $1; }
				;
for_step		: %empty						{ $$ = NULL; }
				| expr							{ $$ = expr_create(EXPR_ARG, $1, NULL); }
				| for_step ',' expr				{ $$ = expr_create(EXPR_ARG, $3, $1); }
				;
print_body		: expr							{ $$ = expr_create(EXPR_ARG, $1, NULL); }
				| print_body ',' expr			{ $$ = expr_create(EXPR_ARG, $3, $1); }
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
				| '-' expr			%prec '!'	{ $$ = expr_create(EXPR_SUB, 0, $2); }
				| '!' expr						{ $$ = expr_create(EXPR_NOT, $2, NULL); }
				| expr "++"						{ $$ = expr_create(EXPR_INC, $1, NULL); }
				| expr "--"						{ $$ = expr_create(EXPR_DEC, $1, NULL); }
				;
param_expr		: %empty						{ $$ = NULL; }
				| expr							{ $$ = expr_create(EXPR_ARG, $1, NULL); }
				| param_expr ',' expr			{ $$ = expr_create(EXPR_ARG, $3, $1); }

%%

void yyerror(const char *s)
{
  extern int yylineno;	// Defined and maintained in scanner.c
  extern char *yytext;	// Defined and maintained in scanner.c
  
  fprintf(stderr, "ERROR: %s at symbol \"%s\" on line %d\n", s, yytext, yylineno);
  exit(1);
}

int main(int argc, char *argv[]) {
	char *inputFile;
	
	if (argc > 1) inputFile = argv[1];
	
	yyin = fopen(inputFile, "r");
	if (!yyin) {
		printf("Could not open: %s\n", inputFile);
		return 1;
	}
	
	if (yyparse()==0) {
		printf("Parse successful!\n");
	} else {
		printf("Parse unsuccessful :c\n");
	}
	
	fclose(yyin);
	decl_free(result); // Free up AST
	return 0;
}
