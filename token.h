/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_TOKEN_H_INCLUDED
# define YY_YY_TOKEN_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    IDENTIFIER = 258,
    CHAR_LITERAL = 259,
    INTEGER_LITERAL = 260,
    STRING_LITERAL = 261,
    ARRAY = 262,
    BOOLEAN = 263,
    CHAR = 264,
    FUNCTION = 265,
    INTEGER = 266,
    STRING = 267,
    VOID = 268,
    ELSE = 269,
    FALSE = 270,
    FOR = 271,
    IF = 272,
    PRINT = 273,
    RETURN = 274,
    TRUE = 275,
    WHILE = 276,
    EQUAL = 277,
    PLUS = 278,
    MINUS = 279,
    MULTIPLY = 280,
    DIVIDE = 281,
    MODULO = 282,
    EXPONENTIATE = 283,
    AND = 284,
    OR = 285,
    LT = 286,
    LT_EQ = 287,
    GT = 288,
    GT_EQ = 289,
    IS_EQ = 290,
    IS_NEQ = 291,
    POST_INC = 292,
    POST_DEC = 293,
    NEGATE = 294,
    LC_PAREN = 295,
    RC_PAREN = 296,
    LR_PAREN = 297,
    RR_PAREN = 298,
    LS_PAREN = 299,
    RS_PAREN = 300,
    COLON = 301,
    SEMICOLON = 302,
    QUOTE = 303,
    DBQUOTE = 304,
    COMMA = 305
  };
#endif

/* Value type.  */


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_TOKEN_H_INCLUDED  */
