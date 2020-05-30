%{

#include <stdlib.h>
#include "type.h"
#include "token.h"

char strip_quotes_char(const char *token) {
	return token[1];
}

char *strip_quotes_str(const char *token, int token_length) {
	char *result = malloc(token_length - 1);
	memcpy(result, token + 1, token_length - 2);
	result[token_length - 2] = 0;
	return result;
}

%}

INTEGER	(0|[1-9][0-9]*)	
ID		[a-zA-Z][a-zA-Z0-9_]*

%%
\/\/.*(\n|\r|\r\n)	{ yylineno++; }			/* Skip comment */
\/\*([^*]|\*+[^/])*\*+\/					/* Skip multiline comment, TODO count newlines */
[ \t\b\v]+									/* Skip whitespace */
(\n|\r|\r\n)		{ yylineno++; }			/* Skip whitespace */
\'.\'				{ yylval.cval = strip_quotes_char(yytext); 		  return CHAR_LITERAL; } // TODO accept control characters like \n
{INTEGER}			{ yylval.ival = atoi(yytext); 					  return INTEGER_LITERAL; }
\"[^"]*\"			{ yylval.sval = strip_quotes_str(yytext, yyleng); return STRING_LITERAL; }
array				{ return ARRAY; }
boolean				{ return BOOLEAN; }
char				{ return CHAR; }
else				{ return ELSE; }
false				{ return FALSE; }
for					{ return FOR; }
function			{ return FUNCTION; }
if					{ return IF; }
integer				{ return INTEGER; }
print				{ return PRINT; }
return				{ return RETURN; }
string				{ return STRING; }
true				{ return TRUE; }
while				{ return WHILE; }
void				{ return VOID; }
{ID}				{ yylval.sval = strdup(yytext); return IDENTIFIER; }
"&&"				{ return AND; }			/* Multichar operators */
"||"				{ return OR; }
"<="				{ return LT_EQ; }
"=>"				{ return GT_EQ; }
"=="				{ return IS_EQ; }
"!="				{ return IS_NEQ; }
"++"				{ return POST_INC; }
"--"				{ return POST_DEC; }
.					{ return(yytext[0]); }	/* Any single unmatched char */

%%

/* Indicate that we're done lexing */
int yywrap() { return 1; }
