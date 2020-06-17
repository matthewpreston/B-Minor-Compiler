/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.bison"


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


#line 120 "parser.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   489

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  72
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  32
/* YYNRULES -- Number of rules.  */
#define YYNRULES  97
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  191

#define YYUNDEFTOK  2
#define YYMAXUTOK   305


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    49,    69,     2,     2,    33,     2,    67,
      55,    57,    29,    25,    71,    27,     2,    31,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    63,    65,
      39,    23,    42,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    59,     2,    61,    35,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    51,     2,    53,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    24,    26,
      28,    30,    32,    34,    36,    37,    38,    40,    41,    43,
      44,    45,    46,    47,    48,    50,    52,    54,    56,    58,
      60,    62,    64,    66,    68,    70
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   103,   103,   103,   105,   106,   108,   109,   111,   114,
     121,   122,   123,   124,   126,   127,   129,   130,   133,   134,
     138,   139,   140,   141,   144,   145,   146,   149,   150,   151,
     153,   155,   156,   158,   159,   161,   162,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   193,   194,   196,   197,   200,
     207,   208,   212,   214,   215,   217,   218,   220,   221,   223,
     225,   229,   232,   238,   239,   243,   246,   252,   254,   256,
     258,   260,   264,   265,   267,   268,   270,   271
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "CHAR_LITERAL",
  "INTEGER_LITERAL", "STRING_LITERAL", "\"array\"", "\"boolean\"",
  "\"char\"", "\"function\"", "\"integer\"", "\"string\"", "\"void\"",
  "\"else\"", "\"false\"", "\"for\"", "\"if\"", "\"print\"", "\"return\"",
  "\"true\"", "\"while\"", "EQUAL", "'='", "PLUS", "'+'", "MINUS", "'-'",
  "MULTIPLY", "'*'", "DIVIDE", "'/'", "MODULO", "'%'", "EXPONENTIATE",
  "'^'", "\"&&\"", "\"||\"", "LT", "'<'", "\"<=\"", "GT", "'>'", "\">=\"",
  "\"==\"", "\"!=\"", "\"++\"", "\"--\"", "NEGATE", "'!'", "LC_PAREN",
  "'{'", "RC_PAREN", "'}'", "LR_PAREN", "'('", "RR_PAREN", "')'",
  "LS_PAREN", "'['", "RS_PAREN", "']'", "COLON", "':'", "SEMICOLON", "';'",
  "QUOTE", "'\\''", "DBQUOTE", "'\"'", "COMMA", "','", "$accept",
  "program", "$@1", "decls", "decl", "variable", "basic_type",
  "basic_var_assign", "array_type", "type", "return_type", "arr_ret_type",
  "param_list", "param", "array_assign", "array_init", "one_or_more_expr",
  "expr", "param_expr", "nested_arr_init", "function", "func_init",
  "scope_enter", "comp_statement", "one_or_more_stmt", "statement",
  "open_statement", "closed_statement", "simple_statement", "for_init",
  "for_cond", "for_step", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,    61,   278,    43,   279,    45,   280,    42,
     281,    47,   282,    37,   283,    94,   284,   285,   286,    60,
     287,   288,    62,   289,   290,   291,   292,   293,   294,    33,
     295,   123,   296,   125,   297,    40,   298,    41,   299,    91,
     300,    93,   301,    58,   302,    59,   303,    39,   304,    34,
     305,    44
};
# endif

#define YYPACT_NINF (-167)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -167,     2,     7,  -167,   -52,     7,  -167,   -45,  -167,   195,
    -167,  -167,   -38,  -167,  -167,   158,  -167,  -167,    23,    18,
     -11,   158,  -167,  -167,    -5,   137,  -167,    -9,     6,    -4,
      56,  -167,  -167,  -167,  -167,  -167,  -167,   137,   137,   137,
     343,   455,   477,    56,     9,   -15,  -167,    75,    75,   268,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,  -167,  -167,   137,   137,    11,
     158,  -167,    50,  -167,    15,   158,  -167,  -167,   -10,   158,
       3,    56,  -167,   368,   424,   424,    58,    58,    58,    75,
     391,   391,   414,   414,   414,   414,   414,   414,  -167,   154,
      19,   241,    72,    34,    27,  -167,    29,    36,  -167,  -167,
    -167,  -167,  -167,    80,  -167,   137,  -167,  -167,    31,    56,
      60,   477,    56,   -52,    39,    48,   137,   137,    55,  -167,
     187,    80,    59,    80,  -167,  -167,  -167,  -167,  -167,   455,
      -2,    60,    61,  -167,    47,  -167,     0,   137,   137,    54,
     214,   137,  -167,    71,  -167,  -167,  -167,  -167,    74,  -167,
      77,  -167,  -167,    67,   293,  -167,  -167,   318,  -167,  -167,
      60,   137,    80,    80,    83,   343,    68,  -167,   111,  -167,
    -167,  -167,   137,    80,  -167,    82,  -167,  -167,    80,  -167,
    -167
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     0,     1,     0,     3,     4,     0,     7,     0,
       5,     6,     0,    10,    11,     0,    12,    13,    14,     0,
       0,     0,    21,    20,     0,     0,     8,     0,     0,     0,
      27,    37,    38,    39,    40,    41,    42,     0,     0,     0,
      15,     0,     0,    27,     0,     0,    28,    61,    62,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,     0,     0,
       0,    18,    31,    16,     0,     0,    24,    22,     0,     0,
       0,     0,    43,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    66,    35,
       0,     0,     0,     0,     0,     9,     0,     0,    23,    30,
      72,    70,    69,    73,    29,     0,    45,    44,     0,    27,
       0,     0,    27,    37,     0,     0,     0,     0,     0,    87,
       0,    73,     0,    74,    75,    77,    78,    83,    36,     0,
       0,     0,     0,    33,    34,    25,     0,    92,     0,     0,
       0,     0,    88,     0,    71,    76,    19,    17,     0,    32,
       0,    26,    93,     0,     0,    89,    90,     0,    91,    67,
       0,    94,     0,     0,     0,    95,     0,    79,    78,    81,
      85,    68,    96,     0,    97,     0,    80,    84,     0,    82,
      86
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -167,  -167,  -167,  -167,    13,  -167,    -1,  -167,     5,  -167,
     -17,    16,   -40,    64,  -167,  -125,   -66,   -25,  -167,  -167,
    -167,  -167,    69,    20,  -167,  -128,  -164,  -166,  -167,  -167,
    -167,  -167
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,     5,   129,     7,    23,    26,    72,    73,
      24,    77,    45,    46,   105,   142,   143,    99,   100,   144,
       8,   112,   131,   132,   133,   134,   135,   136,   137,   163,
     176,   185
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      40,    98,     3,    78,    29,   155,   178,   180,    18,   179,
       4,     9,    47,    48,    49,     6,   158,   187,    10,   186,
      11,    19,   190,    27,   189,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      71,    76,    80,   101,   177,   174,    25,   108,    28,   138,
      30,    43,    41,   103,   110,   157,    81,   161,   107,    44,
     149,    81,   109,    31,    32,    33,    34,    42,   111,    81,
     102,    81,    79,   104,   106,    35,   116,   118,   120,   140,
      36,   162,   146,   123,    32,    33,    34,    37,   130,   119,
     121,   122,   139,    56,   147,    35,   124,   125,   126,   127,
      36,   128,   150,   148,    65,    66,   130,    37,   130,    38,
     151,   141,   154,    67,   159,    39,   184,    68,   160,   165,
      76,    65,    66,   164,   168,   183,   167,   169,   170,    38,
      67,   110,   171,   182,    68,    39,   181,   145,    71,   188,
      31,    32,    33,    34,   156,   114,   175,   130,   130,   113,
       0,   153,    35,     0,     0,     0,     0,    36,   130,     0,
       0,     0,     0,   130,    37,    20,    13,    14,    21,    16,
      17,    22,     0,     0,     0,     0,     0,    50,     0,    51,
       0,    52,     0,    53,     0,    54,    38,    55,     0,    56,
      57,    58,    39,    59,    60,     0,    61,    62,    63,    64,
      65,    66,    12,    13,    14,    15,    16,    17,     0,    67,
      50,     0,    51,    68,    52,     0,    53,     0,    54,     0,
      55,     0,    56,    57,    58,   115,    59,    60,     0,    61,
      62,    63,    64,    65,    66,     0,     0,    50,     0,    51,
       0,    52,    67,    53,     0,    54,    68,    55,     0,    56,
      57,    58,   152,    59,    60,     0,    61,    62,    63,    64,
      65,    66,     0,     0,    50,     0,    51,     0,    52,    67,
      53,     0,    54,    68,    55,     0,    56,    57,    58,   166,
      59,    60,     0,    61,    62,    63,    64,    65,    66,     0,
       0,    50,     0,    51,     0,    52,    67,    53,     0,    54,
      68,    55,   117,    56,    57,    58,     0,    59,    60,     0,
      61,    62,    63,    64,    65,    66,    50,     0,    51,     0,
      52,     0,    53,    67,    54,    82,    55,    68,    56,    57,
      58,     0,    59,    60,     0,    61,    62,    63,    64,    65,
      66,    50,     0,    51,     0,    52,     0,    53,    67,    54,
     172,    55,    68,    56,    57,    58,     0,    59,    60,     0,
      61,    62,    63,    64,    65,    66,    50,     0,    51,     0,
      52,     0,    53,    67,    54,   173,    55,    68,    56,    57,
      58,     0,    59,    60,     0,    61,    62,    63,    64,    65,
      66,     0,     0,    51,     0,    52,     0,    53,    67,    54,
       0,    55,    68,    56,    57,    58,     0,    59,    60,     0,
      61,    62,    63,    64,    65,    66,    51,     0,    52,     0,
      53,     0,    54,    67,    55,     0,    56,    68,     0,     0,
      59,    60,     0,    61,    62,    63,    64,    65,    66,    51,
       0,    52,     0,    53,     0,    54,    67,    55,     0,    56,
      68,     0,     0,    53,     0,    54,     0,    55,     0,    56,
      65,    66,    69,    13,    14,    70,    16,    17,     0,    67,
      65,    66,     0,    68,     0,     0,     0,     0,     0,    67,
       0,     0,     0,    68,    74,    13,    14,    75,    16,    17
};

static const yytype_int16 yycheck[] =
{
      25,    67,     0,    43,    21,   133,   172,   173,     9,   173,
       3,    63,    37,    38,    39,     2,   141,   183,     5,   183,
      65,    59,   188,     5,   188,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      41,    42,    57,    68,   172,   170,    23,    57,    59,   115,
      55,    55,    61,    70,    51,    57,    71,    57,    75,     3,
     126,    71,    79,     3,     4,     5,     6,    61,    65,    71,
      59,    71,    63,    23,    59,    15,    57,     5,    51,   119,
      20,   147,   122,     3,     4,     5,     6,    27,   113,    55,
      61,    55,    61,    35,    55,    15,    16,    17,    18,    19,
      20,    21,   127,    55,    46,    47,   131,    27,   133,    49,
      55,    51,    53,    55,    53,    55,   182,    59,    71,    65,
     121,    46,    47,   148,    53,    14,   151,    53,    51,    49,
      55,    51,    65,    65,    59,    55,    53,   121,   139,    57,
       3,     4,     5,     6,   139,    81,   171,   172,   173,    80,
      -1,   131,    15,    -1,    -1,    -1,    -1,    20,   183,    -1,
      -1,    -1,    -1,   188,    27,     7,     8,     9,    10,    11,
      12,    13,    -1,    -1,    -1,    -1,    -1,    23,    -1,    25,
      -1,    27,    -1,    29,    -1,    31,    49,    33,    -1,    35,
      36,    37,    55,    39,    40,    -1,    42,    43,    44,    45,
      46,    47,     7,     8,     9,    10,    11,    12,    -1,    55,
      23,    -1,    25,    59,    27,    -1,    29,    -1,    31,    -1,
      33,    -1,    35,    36,    37,    71,    39,    40,    -1,    42,
      43,    44,    45,    46,    47,    -1,    -1,    23,    -1,    25,
      -1,    27,    55,    29,    -1,    31,    59,    33,    -1,    35,
      36,    37,    65,    39,    40,    -1,    42,    43,    44,    45,
      46,    47,    -1,    -1,    23,    -1,    25,    -1,    27,    55,
      29,    -1,    31,    59,    33,    -1,    35,    36,    37,    65,
      39,    40,    -1,    42,    43,    44,    45,    46,    47,    -1,
      -1,    23,    -1,    25,    -1,    27,    55,    29,    -1,    31,
      59,    33,    61,    35,    36,    37,    -1,    39,    40,    -1,
      42,    43,    44,    45,    46,    47,    23,    -1,    25,    -1,
      27,    -1,    29,    55,    31,    57,    33,    59,    35,    36,
      37,    -1,    39,    40,    -1,    42,    43,    44,    45,    46,
      47,    23,    -1,    25,    -1,    27,    -1,    29,    55,    31,
      57,    33,    59,    35,    36,    37,    -1,    39,    40,    -1,
      42,    43,    44,    45,    46,    47,    23,    -1,    25,    -1,
      27,    -1,    29,    55,    31,    57,    33,    59,    35,    36,
      37,    -1,    39,    40,    -1,    42,    43,    44,    45,    46,
      47,    -1,    -1,    25,    -1,    27,    -1,    29,    55,    31,
      -1,    33,    59,    35,    36,    37,    -1,    39,    40,    -1,
      42,    43,    44,    45,    46,    47,    25,    -1,    27,    -1,
      29,    -1,    31,    55,    33,    -1,    35,    59,    -1,    -1,
      39,    40,    -1,    42,    43,    44,    45,    46,    47,    25,
      -1,    27,    -1,    29,    -1,    31,    55,    33,    -1,    35,
      59,    -1,    -1,    29,    -1,    31,    -1,    33,    -1,    35,
      46,    47,     7,     8,     9,    10,    11,    12,    -1,    55,
      46,    47,    -1,    59,    -1,    -1,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    59,     7,     8,     9,    10,    11,    12
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    73,    74,     0,     3,    75,    76,    77,    92,    63,
      76,    65,     7,     8,     9,    10,    11,    12,    78,    59,
       7,    10,    13,    78,    82,    23,    79,     5,    59,    82,
      55,     3,     4,     5,     6,    15,    20,    27,    49,    55,
      89,    61,    61,    55,     3,    84,    85,    89,    89,    89,
      23,    25,    27,    29,    31,    33,    35,    36,    37,    39,
      40,    42,    43,    44,    45,    46,    47,    55,    59,     7,
      10,    78,    80,    81,     7,    10,    78,    83,    84,    63,
      57,    71,    57,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    88,    89,
      90,    89,    59,    82,    23,    86,    59,    82,    57,    82,
      51,    65,    93,    94,    85,    71,    57,    61,     5,    55,
      51,    61,    55,     3,    16,    17,    18,    19,    21,    76,
      89,    94,    95,    96,    97,    98,    99,   100,    88,    61,
      84,    51,    87,    88,    91,    83,    84,    55,    55,    88,
      89,    55,    65,    95,    53,    97,    80,    57,    87,    53,
      71,    57,    88,   101,    89,    65,    65,    89,    53,    53,
      51,    65,    57,    57,    87,    89,   102,    97,    99,    98,
      99,    53,    65,    14,    88,   103,    98,    99,    57,    98,
      99
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    72,    74,    73,    75,    75,    76,    76,    77,    77,
      78,    78,    78,    78,    79,    79,    80,    80,    81,    81,
      82,    82,    82,    82,    83,    83,    83,    84,    84,    84,
      85,    86,    86,    87,    87,    88,    88,    89,    89,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    90,    90,    91,    91,    92,
      93,    93,    94,    95,    95,    96,    96,    97,    97,    98,
      98,    98,    98,    99,    99,    99,    99,   100,   100,   100,
     100,   100,   101,   101,   102,   102,   103,   103
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     1,     2,     2,     1,     4,     8,
       1,     1,     1,     1,     0,     2,     1,     5,     1,     5,
       1,     1,     4,     5,     1,     4,     5,     0,     1,     3,
       3,     0,     4,     1,     1,     1,     3,     1,     1,     1,
       1,     1,     1,     3,     4,     4,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     2,     0,     1,     3,     5,     8,
       1,     3,     1,     0,     1,     1,     2,     1,     1,     5,
       7,     5,     9,     1,     7,     5,     9,     1,     2,     3,
       3,     3,     0,     1,     0,     1,     0,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2:
#line 103 "parser.bison"
                          { init_stmt_stack(); }
#line 1589 "parser.c"
    break;

  case 3:
#line 103 "parser.bison"
                                                        { result = (yyvsp[0].decl); free_stmt_stack(); }
#line 1595 "parser.c"
    break;

  case 4:
#line 105 "parser.bison"
                                                                                { (yyval.decl) = (yyvsp[0].decl); prev_decl = (yyvsp[0].decl); }
#line 1601 "parser.c"
    break;

  case 5:
#line 106 "parser.bison"
                                                                                { (yyval.decl) = (yyvsp[-1].decl); prev_decl->next = (yyvsp[0].decl); prev_decl = (yyvsp[0].decl); }
#line 1607 "parser.c"
    break;

  case 6:
#line 108 "parser.bison"
                                                                        { (yyval.decl) = (yyvsp[-1].decl); }
#line 1613 "parser.c"
    break;

  case 7:
#line 109 "parser.bison"
                                                                                        { (yyval.decl) = (yyvsp[0].decl); }
#line 1619 "parser.c"
    break;

  case 8:
#line 113 "parser.bison"
                                                                                { (yyval.decl) = decl_create((yyvsp[-3].sval), (yyvsp[-1].type), (yyvsp[0].expr), NULL, NULL); }
#line 1625 "parser.c"
    break;

  case 9:
#line 117 "parser.bison"
                                                                        { (yyval.decl) = decl_create((yyvsp[-7].sval),
													type_create(TYPE_ARRAY, (yyvsp[-1].type), NULL),
													(yyvsp[0].expr), NULL, NULL); }
#line 1633 "parser.c"
    break;

  case 10:
#line 121 "parser.bison"
                                                                                { (yyval.type) = type_create(TYPE_BOOLEAN, NULL, NULL); }
#line 1639 "parser.c"
    break;

  case 11:
#line 122 "parser.bison"
                                                                                        { (yyval.type) = type_create(TYPE_CHAR, NULL, NULL); }
#line 1645 "parser.c"
    break;

  case 12:
#line 123 "parser.bison"
                                                                                        { (yyval.type) = type_create(TYPE_INTEGER, NULL, NULL); }
#line 1651 "parser.c"
    break;

  case 13:
#line 124 "parser.bison"
                                                                                        { (yyval.type) = type_create(TYPE_STRING, NULL, NULL); }
#line 1657 "parser.c"
    break;

  case 14:
#line 126 "parser.bison"
                                                                        { (yyval.expr) = NULL; }
#line 1663 "parser.c"
    break;

  case 15:
#line 127 "parser.bison"
                                                                                        { (yyval.expr) = (yyvsp[0].expr); }
#line 1669 "parser.c"
    break;

  case 16:
#line 129 "parser.bison"
                                                                                { (yyval.type) = (yyvsp[0].type); }
#line 1675 "parser.c"
    break;

  case 17:
#line 131 "parser.bison"
                                                                                { (yyval.type) = type_create(TYPE_FUNCTION, (yyvsp[-3].type), (yyvsp[-1].param_list)); }
#line 1681 "parser.c"
    break;

  case 18:
#line 133 "parser.bison"
                                                                        { (yyval.type) = (yyvsp[0].type); }
#line 1687 "parser.c"
    break;

  case 19:
#line 136 "parser.bison"
                                                                                        { (yyval.type) = type_create(TYPE_ARRAY, (yyvsp[0].type), NULL); }
#line 1693 "parser.c"
    break;

  case 20:
#line 138 "parser.bison"
                                                                        { (yyval.type) = (yyvsp[0].type); }
#line 1699 "parser.c"
    break;

  case 21:
#line 139 "parser.bison"
                                                                                        { (yyval.type) = type_create(TYPE_VOID, NULL, NULL); }
#line 1705 "parser.c"
    break;

  case 22:
#line 140 "parser.bison"
                                                                { (yyval.type) = type_create(TYPE_ARRAY, (yyvsp[0].type), NULL); }
#line 1711 "parser.c"
    break;

  case 23:
#line 142 "parser.bison"
                                                                                { (yyval.type) = type_create(TYPE_FUNCTION, (yyvsp[-3].type), (yyvsp[-1].param_list)); }
#line 1717 "parser.c"
    break;

  case 24:
#line 144 "parser.bison"
                                                                { (yyval.type) = (yyvsp[0].type); }
#line 1723 "parser.c"
    break;

  case 25:
#line 145 "parser.bison"
                                                                { (yyval.type) = type_create(TYPE_ARRAY, (yyvsp[0].type), NULL); }
#line 1729 "parser.c"
    break;

  case 26:
#line 147 "parser.bison"
                                                                                { (yyval.type) = type_create(TYPE_FUNCTION, (yyvsp[-3].type), (yyvsp[-1].param_list)); }
#line 1735 "parser.c"
    break;

  case 27:
#line 149 "parser.bison"
                                                                                { (yyval.param_list) = NULL; }
#line 1741 "parser.c"
    break;

  case 28:
#line 150 "parser.bison"
                                                                                        { (yyval.param_list) = (yyvsp[0].param_list); prev_param = (yyvsp[0].param_list); }
#line 1747 "parser.c"
    break;

  case 29:
#line 151 "parser.bison"
                                                                        { (yyval.param_list) = (yyvsp[-2].param_list); prev_param->next = (yyvsp[0].param_list); prev_param = (yyvsp[0].param_list); }
#line 1753 "parser.c"
    break;

  case 30:
#line 153 "parser.bison"
                                                        { (yyval.param_list) = param_list_create((yyvsp[-2].sval), (yyvsp[0].type), NULL); }
#line 1759 "parser.c"
    break;

  case 31:
#line 155 "parser.bison"
                                                                        { (yyval.expr) = NULL; }
#line 1765 "parser.c"
    break;

  case 32:
#line 156 "parser.bison"
                                                                        { (yyval.expr) = (yyvsp[-1].expr); }
#line 1771 "parser.c"
    break;

  case 33:
#line 158 "parser.bison"
                                                                        { (yyval.expr) = (yyvsp[0].expr); }
#line 1777 "parser.c"
    break;

  case 34:
#line 159 "parser.bison"
                                                                                { (yyval.expr) = (yyvsp[0].expr); }
#line 1783 "parser.c"
    break;

  case 35:
#line 161 "parser.bison"
                                                                        { (yyval.expr) = expr_create(EXPR_ARG, (yyvsp[0].expr), NULL); }
#line 1789 "parser.c"
    break;

  case 36:
#line 162 "parser.bison"
                                                                        { (yyval.expr) = expr_create(EXPR_ARG, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1795 "parser.c"
    break;

  case 37:
#line 164 "parser.bison"
                                                                        { (yyval.expr) = expr_create_identifier((yyvsp[0].sval)); }
#line 1801 "parser.c"
    break;

  case 38:
#line 165 "parser.bison"
                                                                                { (yyval.expr) = expr_create_char_literal((yyvsp[0].cval)); }
#line 1807 "parser.c"
    break;

  case 39:
#line 166 "parser.bison"
                                                                                { (yyval.expr) = expr_create_integer_literal((yyvsp[0].ival)); }
#line 1813 "parser.c"
    break;

  case 40:
#line 167 "parser.bison"
                                                                                { (yyval.expr) = expr_create_string_literal((yyvsp[0].sval)); }
#line 1819 "parser.c"
    break;

  case 41:
#line 168 "parser.bison"
                                                                                        { (yyval.expr) = expr_create_boolean_literal(0); }
#line 1825 "parser.c"
    break;

  case 42:
#line 169 "parser.bison"
                                                                                        { (yyval.expr) = expr_create_boolean_literal(1); }
#line 1831 "parser.c"
    break;

  case 43:
#line 170 "parser.bison"
                                                                                { (yyval.expr) = (yyvsp[-1].expr); }
#line 1837 "parser.c"
    break;

  case 44:
#line 171 "parser.bison"
                                                                                { (yyval.expr) = expr_create(EXPR_SUBSCRIPT, (yyvsp[-3].expr), (yyvsp[-1].expr)); }
#line 1843 "parser.c"
    break;

  case 45:
#line 172 "parser.bison"
                                                                        { (yyval.expr) = expr_create(EXPR_CALL, (yyvsp[-3].expr), (yyvsp[-1].expr)); }
#line 1849 "parser.c"
    break;

  case 46:
#line 173 "parser.bison"
                                                                                { (yyval.expr) = expr_create(EXPR_ASSIGN, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1855 "parser.c"
    break;

  case 47:
#line 174 "parser.bison"
                                                                                { (yyval.expr) = expr_create(EXPR_ADD, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1861 "parser.c"
    break;

  case 48:
#line 175 "parser.bison"
                                                                                { (yyval.expr) = expr_create(EXPR_SUB, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1867 "parser.c"
    break;

  case 49:
#line 176 "parser.bison"
                                                                                { (yyval.expr) = expr_create(EXPR_MUL, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1873 "parser.c"
    break;

  case 50:
#line 177 "parser.bison"
                                                                                { (yyval.expr) = expr_create(EXPR_DIV, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1879 "parser.c"
    break;

  case 51:
#line 178 "parser.bison"
                                                                                { (yyval.expr) = expr_create(EXPR_MOD, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1885 "parser.c"
    break;

  case 52:
#line 179 "parser.bison"
                                                                                { (yyval.expr) = expr_create(EXPR_EXP, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1891 "parser.c"
    break;

  case 53:
#line 180 "parser.bison"
                                                                                { (yyval.expr) = expr_create(EXPR_AND, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1897 "parser.c"
    break;

  case 54:
#line 181 "parser.bison"
                                                                                { (yyval.expr) = expr_create(EXPR_OR, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1903 "parser.c"
    break;

  case 55:
#line 182 "parser.bison"
                                                                                { (yyval.expr) = expr_create(EXPR_LT, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1909 "parser.c"
    break;

  case 56:
#line 183 "parser.bison"
                                                                                { (yyval.expr) = expr_create(EXPR_LT_EQ, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1915 "parser.c"
    break;

  case 57:
#line 184 "parser.bison"
                                                                                { (yyval.expr) = expr_create(EXPR_GT, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1921 "parser.c"
    break;

  case 58:
#line 185 "parser.bison"
                                                                                { (yyval.expr) = expr_create(EXPR_GT_EQ, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1927 "parser.c"
    break;

  case 59:
#line 186 "parser.bison"
                                                                                { (yyval.expr) = expr_create(EXPR_IS_EQ, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1933 "parser.c"
    break;

  case 60:
#line 187 "parser.bison"
                                                                                { (yyval.expr) = expr_create(EXPR_IS_NEQ, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1939 "parser.c"
    break;

  case 61:
#line 188 "parser.bison"
                                                                                { (yyval.expr) = expr_create(EXPR_SUB, 0, (yyvsp[0].expr)); }
#line 1945 "parser.c"
    break;

  case 62:
#line 189 "parser.bison"
                                                                                        { (yyval.expr) = expr_create(EXPR_NOT, (yyvsp[0].expr), NULL); }
#line 1951 "parser.c"
    break;

  case 63:
#line 190 "parser.bison"
                                                                                        { (yyval.expr) = expr_create(EXPR_INC, (yyvsp[-1].expr), NULL); }
#line 1957 "parser.c"
    break;

  case 64:
#line 191 "parser.bison"
                                                                                        { (yyval.expr) = expr_create(EXPR_DEC, (yyvsp[-1].expr), NULL); }
#line 1963 "parser.c"
    break;

  case 65:
#line 193 "parser.bison"
                                                                                { (yyval.expr) = NULL; }
#line 1969 "parser.c"
    break;

  case 66:
#line 194 "parser.bison"
                                                                                { (yyval.expr) = (yyvsp[0].expr); }
#line 1975 "parser.c"
    break;

  case 67:
#line 196 "parser.bison"
                                                        { (yyval.expr) = (yyvsp[-1].expr); }
#line 1981 "parser.c"
    break;

  case 68:
#line 198 "parser.bison"
                                                                        { (yyval.expr) = expr_create(EXPR_ARG, (yyvsp[-4].expr), (yyvsp[-1].expr)); }
#line 1987 "parser.c"
    break;

  case 69:
#line 203 "parser.bison"
                                                                                        { (yyval.decl) = decl_create((yyvsp[-7].sval),
													type_create(TYPE_FUNCTION, (yyvsp[-4].type), (yyvsp[-2].param_list)),
													NULL, (yyvsp[0].stmt), NULL); }
#line 1995 "parser.c"
    break;

  case 70:
#line 207 "parser.bison"
                                                                                { (yyval.stmt) = NULL; }
#line 2001 "parser.c"
    break;

  case 71:
#line 208 "parser.bison"
                                                                { (yyval.stmt) = stmt_create(STMT_BLOCK, NULL, NULL, NULL,
													NULL, (yyvsp[-1].stmt), NULL, NULL);
												  exit_scope_stmt(); }
#line 2009 "parser.c"
    break;

  case 72:
#line 212 "parser.bison"
                                                                                { enter_scope_stmt(); }
#line 2015 "parser.c"
    break;

  case 73:
#line 214 "parser.bison"
                                                                        { (yyval.stmt) = NULL; }
#line 2021 "parser.c"
    break;

  case 74:
#line 215 "parser.bison"
                                                                                { (yyval.stmt) = (yyvsp[0].stmt); }
#line 2027 "parser.c"
    break;

  case 75:
#line 217 "parser.bison"
                                                                        { (yyval.stmt) = (yyvsp[0].stmt); init_prev_stmt((yyvsp[0].stmt)); }
#line 2033 "parser.c"
    break;

  case 76:
#line 218 "parser.bison"
                                                                { (yyval.stmt) = (yyvsp[-1].stmt); link_prev_stmt((yyvsp[0].stmt)); }
#line 2039 "parser.c"
    break;

  case 77:
#line 220 "parser.bison"
                                                                        { (yyval.stmt) = (yyvsp[0].stmt); }
#line 2045 "parser.c"
    break;

  case 78:
#line 221 "parser.bison"
                                                                                { (yyval.stmt) = (yyvsp[0].stmt); }
#line 2051 "parser.c"
    break;

  case 79:
#line 223 "parser.bison"
                                                { (yyval.stmt) = stmt_create(STMT_IF_ELSE, NULL, NULL, (yyvsp[-2].expr),
													NULL, (yyvsp[0].stmt), NULL, NULL); }
#line 2058 "parser.c"
    break;

  case 80:
#line 227 "parser.bison"
                                                                        { (yyval.stmt) = stmt_create(STMT_IF_ELSE, NULL, NULL, (yyvsp[-4].expr),
													NULL, (yyvsp[-2].stmt), (yyvsp[0].stmt), NULL); }
#line 2065 "parser.c"
    break;

  case 81:
#line 230 "parser.bison"
                                                                                { (yyval.stmt) = stmt_create(STMT_FOR, NULL, NULL, (yyvsp[-2].expr), NULL,
													(yyvsp[0].stmt), NULL, NULL); }
#line 2072 "parser.c"
    break;

  case 82:
#line 235 "parser.bison"
                                                                                { (yyval.stmt) = stmt_create(STMT_FOR, NULL, (yyvsp[-6].expr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].stmt),
													NULL, NULL); }
#line 2079 "parser.c"
    break;

  case 83:
#line 238 "parser.bison"
                                                                { (yyval.stmt) = (yyvsp[0].stmt); }
#line 2085 "parser.c"
    break;

  case 84:
#line 241 "parser.bison"
                                                                        { (yyval.stmt) = stmt_create(STMT_IF_ELSE, NULL, NULL, (yyvsp[-4].expr),
													NULL, (yyvsp[-2].stmt), (yyvsp[0].stmt), NULL); }
#line 2092 "parser.c"
    break;

  case 85:
#line 244 "parser.bison"
                                                                                { (yyval.stmt) = stmt_create(STMT_FOR, NULL, NULL, (yyvsp[-2].expr), NULL,
													(yyvsp[0].stmt), NULL, NULL); }
#line 2099 "parser.c"
    break;

  case 86:
#line 249 "parser.bison"
                                                                                { (yyval.stmt) = stmt_create(STMT_FOR, NULL, (yyvsp[-6].expr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].stmt),
													NULL, NULL); }
#line 2106 "parser.c"
    break;

  case 87:
#line 252 "parser.bison"
                                                                        { (yyval.stmt) = stmt_create(STMT_DECL, (yyvsp[0].decl), NULL, NULL, NULL,
													NULL, NULL, NULL); }
#line 2113 "parser.c"
    break;

  case 88:
#line 254 "parser.bison"
                                                                                        { (yyval.stmt) = stmt_create(STMT_EXPR, NULL, NULL, (yyvsp[-1].expr), NULL,
													NULL, NULL, NULL); }
#line 2120 "parser.c"
    break;

  case 89:
#line 256 "parser.bison"
                                                                { (yyval.stmt) = stmt_create(STMT_PRINT, NULL, NULL, (yyvsp[-1].expr), NULL,
													NULL, NULL, NULL); }
#line 2127 "parser.c"
    break;

  case 90:
#line 258 "parser.bison"
                                                                                { (yyval.stmt) = stmt_create(STMT_RETURN, NULL, NULL, (yyvsp[-1].expr),
													NULL, NULL, NULL, NULL); }
#line 2134 "parser.c"
    break;

  case 91:
#line 260 "parser.bison"
                                                                { (yyval.stmt) = stmt_create(STMT_BLOCK, NULL, NULL, NULL,
													NULL, (yyvsp[-1].stmt), NULL, NULL);
												  exit_scope_stmt(); }
#line 2142 "parser.c"
    break;

  case 92:
#line 264 "parser.bison"
                                                                                { (yyval.expr) = NULL; }
#line 2148 "parser.c"
    break;

  case 93:
#line 265 "parser.bison"
                                                                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2154 "parser.c"
    break;

  case 94:
#line 267 "parser.bison"
                                                                                { (yyval.expr) = NULL; }
#line 2160 "parser.c"
    break;

  case 95:
#line 268 "parser.bison"
                                                                                        { (yyval.expr) = (yyvsp[0].expr); }
#line 2166 "parser.c"
    break;

  case 96:
#line 270 "parser.bison"
                                                                                { (yyval.expr) = NULL; }
#line 2172 "parser.c"
    break;

  case 97:
#line 271 "parser.bison"
                                                                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2178 "parser.c"
    break;


#line 2182 "parser.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 274 "parser.bison"


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
	if (resolve_names(result) == 0) {
		printf("PASS\n");
	} else {
		printf("FAIL\n");
		fprintf(stderr, "Semantic analysis failed :c\n");
		exit(1);
	}
	
	printf("Front end successful c:\n");
	decl_free(result); // Free up AST
	return 0;
}
