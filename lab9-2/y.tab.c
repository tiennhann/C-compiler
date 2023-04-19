/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "lab9.y" /* yacc.c:339  */

/*
   

 Garrett Turner 
  lab 9 
  12 / 2 /2022 
  Takes decaf code and generates gas assembly. 
 

*/
/* begin specs */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "symtable.h" // added lab7
#include "ast.h" // added lab6
#include "emit.h"

 
 int LEVEL = 0; // Add a new variable called level that allows me to keep track of scoping with the blocks  
 int offset= 0 ; // how much space we are using for the enviroment
 int goffset = 0; // global offset maintain space in method delecarations lab7 video
 int maxoffset;

int yylex(); // to get rid of warning of yylex()

extern int linecount;// This is a linecount counter variable from lex

struct ASTnodetype *Program; // AST global variable 

/* Gets called by yyparse when there is an error in the program.
   The program will use linecount variable from lex print out the line numbers
   where they error occured 
*/
void yyerror (s)  /* Called by yyparse on error */
     char *s;
{
  printf("Error on line %d ", linecount) ; 
  printf (" %s\n", s);
}



#line 112 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
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
    T_ID = 258,
    T_AND = 259,
    T_ASSIGN = 260,
    T_BOOLTYPE = 261,
    T_BREAK = 262,
    T_CONTINUE = 263,
    T_DOT = 264,
    T_ELSE = 265,
    T_EQ = 266,
    T_EXTERN = 267,
    T_FALSE = 268,
    T_FOR = 269,
    T_FUNC = 270,
    T_GEQ = 271,
    T_GT = 272,
    T_IF = 273,
    T_INTTYPE = 274,
    T_INTCONSTANT = 275,
    T_LEFTSHIFT = 276,
    T_LEQ = 277,
    T_NEQ = 278,
    T_NULL = 279,
    T_OR = 280,
    T_PACKAGE = 281,
    T_RETURN = 282,
    T_RIGHTSHIFT = 283,
    T_STRINGTYPE = 284,
    T_STRINGCONSTANT = 285,
    T_TRUE = 286,
    T_VAR = 287,
    T_VOID = 288,
    T_WHILE = 289
  };
#endif
/* Tokens.  */
#define T_ID 258
#define T_AND 259
#define T_ASSIGN 260
#define T_BOOLTYPE 261
#define T_BREAK 262
#define T_CONTINUE 263
#define T_DOT 264
#define T_ELSE 265
#define T_EQ 266
#define T_EXTERN 267
#define T_FALSE 268
#define T_FOR 269
#define T_FUNC 270
#define T_GEQ 271
#define T_GT 272
#define T_IF 273
#define T_INTTYPE 274
#define T_INTCONSTANT 275
#define T_LEFTSHIFT 276
#define T_LEQ 277
#define T_NEQ 278
#define T_NULL 279
#define T_OR 280
#define T_PACKAGE 281
#define T_RETURN 282
#define T_RIGHTSHIFT 283
#define T_STRINGTYPE 284
#define T_STRINGCONSTANT 285
#define T_TRUE 286
#define T_VAR 287
#define T_VOID 288
#define T_WHILE 289

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 53 "lab9.y" /* yacc.c:355  */

   
   int value; 
   char* string;
   struct ASTnodetype *astnode;// added lab6
   enum AST_Decaf_Types asttype; //added lab6
   enum AST_Operators operator;
 

#line 230 "y.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 247 "y.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
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
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   186

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  49
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  46
/* YYNRULES -- Number of rules.  */
#define YYNRULES  98
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  178

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   289

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    48,     2,     2,     2,     2,     2,     2,
      37,    38,    46,    44,    40,    45,     2,    47,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    39,
      43,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    41,     2,    42,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    35,     2,    36,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   110,   110,   135,   138,   145,   167,   170,   172,   177,
     187,   191,   200,   215,   233,   255,   258,   265,   266,   265,
     303,   304,   306,   321,   343,   343,   359,   361,   369,   390,
     413,   415,   423,   424,   425,   426,   427,   428,   429,   430,
     431,   437,   454,   480,   508,   545,   553,   571,   572,   576,
     577,   586,   597,   608,   616,   626,   630,   632,   637,   643,
     649,   653,   654,   674,   675,   676,   677,   678,   679,   682,
     683,   702,   703,   706,   707,   739,   740,   741,   742,   743,
     744,   748,   770,   772,   796,   797,   798,   816,   829,   830,
     833,   834,   837,   838,   841,   847,   854,   862,   867
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_ID", "T_AND", "T_ASSIGN",
  "T_BOOLTYPE", "T_BREAK", "T_CONTINUE", "T_DOT", "T_ELSE", "T_EQ",
  "T_EXTERN", "T_FALSE", "T_FOR", "T_FUNC", "T_GEQ", "T_GT", "T_IF",
  "T_INTTYPE", "T_INTCONSTANT", "T_LEFTSHIFT", "T_LEQ", "T_NEQ", "T_NULL",
  "T_OR", "T_PACKAGE", "T_RETURN", "T_RIGHTSHIFT", "T_STRINGTYPE",
  "T_STRINGCONSTANT", "T_TRUE", "T_VAR", "T_VOID", "T_WHILE", "'{'", "'}'",
  "'('", "')'", "';'", "','", "'['", "']'", "'<'", "'+'", "'-'", "'*'",
  "'/'", "'!'", "$accept", "Program", "Externs", "ExternDefn",
  "ExternTypeList", "ExternTypeList1", "FieldDecls", "FieldDecl",
  "MethodDecls", "MethodDecl", "$@1", "$@2", "IdenList", "IdenList1",
  "Block", "$@3", "VarDecls", "VarDecl", "Statements", "Statement",
  "Assign", "Lvalue", "MethodCall", "MethodArg", "MethodArgs",
  "MethodArgsList", "IfStatement", "WhileStatement", "ForStatement",
  "ReturnStatement", "BreakStatement", "ContinueStatement", "Expr",
  "Simpleexpression", "Relop", "Additiveexpression", "Addop", "Term",
  "Multop", "Factor", "ExternType", "Type", "MethodType", "BoolConstant",
  "ArrayType", "Constant", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   123,   125,    40,    41,    59,
      44,    91,    93,    60,    43,    45,    42,    47,    33
};
# endif

#define YYPACT_NINF -98

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-98)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      18,    28,    33,    30,    18,    57,   -98,    60,   -98,    38,
      46,    80,    51,   -98,   -98,   -98,    47,   -98,    44,   -98,
      85,    74,    51,    22,    80,    17,    87,    64,    74,   -98,
     -98,   -98,    62,   -98,    83,     3,    65,   -98,   -98,   -98,
     -98,    63,    -2,   -98,   -98,    69,     6,   -98,   -98,   -98,
     -98,    68,   105,   -98,   -98,     6,    76,   -98,    72,    22,
     105,   -98,   -98,    81,   -98,   -98,    88,   112,    84,    88,
      17,   -17,    78,    82,    86,    89,    41,    90,   -98,    92,
      84,    91,   117,    93,   -98,   -98,   -98,   -98,   -98,   -98,
     -98,    94,    95,     1,    34,   -98,   -98,   121,    34,    14,
     -98,    34,   -98,   -98,   -98,    34,   -98,   -98,   -98,     7,
     -98,    34,    34,    34,   -98,    96,    97,   -98,   -98,    50,
      24,    49,   -98,   -98,    98,   100,    99,   101,   103,   106,
     107,   -98,    34,   108,   -98,   -98,     1,   -98,   -98,   -98,
     -98,   -98,   -98,   -98,    34,   -98,   -98,    34,   -98,   -98,
     -98,   -98,   -98,   -98,    34,   -98,    34,    81,   -98,   104,
      81,   109,   -98,   -98,    24,    49,   -98,   110,   115,   -98,
     -98,   -98,   121,    81,   114,   -98,    81,   -98
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     0,     0,     3,     0,     1,     0,     4,     0,
       0,     6,    10,    91,    90,    88,     0,     7,     8,    89,
       0,    15,    10,     0,     0,     0,     0,     0,    15,    11,
      92,    93,     0,     9,     0,     0,     0,    17,     2,    16,
       5,     0,     0,    12,    14,     0,     0,    95,    97,    94,
      98,     0,    20,    96,    13,     0,     0,    21,    22,     0,
       0,    18,    23,     0,    24,    19,    26,     0,    30,    26,
       0,    42,     0,     0,     0,     0,     0,     0,    32,     0,
      30,     0,     0,     0,    35,    36,    37,    38,    39,    40,
      27,     0,     0,    47,     0,    58,    59,     0,     0,     0,
      57,     0,    25,    31,    33,     0,    34,    28,    29,    81,
      46,     0,     0,     0,    82,    49,     0,    48,    45,    60,
      61,    69,    73,    84,     0,    42,     0,     0,     0,     0,
       0,    41,     0,     0,    87,    86,    47,    44,    67,    66,
      65,    63,    68,    64,     0,    71,    72,     0,    77,    79,
      78,    80,    75,    76,     0,    43,     0,     0,    56,     0,
       0,     0,    85,    50,    62,    70,    74,     0,    52,    55,
      53,    83,     0,     0,     0,    51,     0,    54
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -98,   -98,   125,   -98,   -98,   113,   126,   -98,   119,   -98,
     -98,   -98,   -98,    71,   -63,   -98,   102,   -98,    70,   -98,
     -96,   -98,   -58,   -98,    19,   -98,   -98,   -98,   -98,   -98,
     -98,   -98,   -92,   -98,   -98,     9,   -98,    10,   -98,   -97,
     -98,   -20,   111,   -98,   116,   118
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,     4,    16,    17,    21,    22,    27,    28,
      45,    63,    56,    57,    78,    66,    68,    69,    79,    80,
      81,    82,   114,   115,   116,   117,    84,    85,    86,    87,
      88,    89,   118,   119,   144,   120,   147,   121,   154,   122,
      18,    19,    32,    50,    36,   123
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      65,   126,   124,    31,   109,    35,   127,   129,    42,   130,
      83,    47,    13,   131,    47,   134,   135,   109,    48,   133,
      93,    48,    83,    13,    94,    14,    53,    47,    13,    49,
       1,   110,    49,     6,    48,    58,    14,   109,   111,    31,
     161,    14,    43,     5,    93,    49,   112,    47,   132,   113,
      91,   111,   128,   148,    48,    30,     7,   166,    34,   112,
       9,   138,   113,    10,   167,    49,   139,   140,   145,   146,
     149,   111,   141,   142,   150,    11,   174,   151,    99,   112,
     100,    12,   113,    20,    24,    23,    13,    71,    25,    26,
      37,    72,    73,   143,   168,   152,   153,   170,    74,    14,
      38,    40,    75,    41,    44,    46,    52,    54,    55,    15,
     175,    76,    60,   177,    59,    70,    64,    95,    77,    64,
      67,    96,   105,    97,   125,   173,    98,   101,   102,     8,
     104,    62,   106,   107,   108,   137,   136,    33,   156,   157,
     155,    94,   158,   169,   159,   160,   162,    39,    29,   172,
     103,   171,   176,   164,     0,   163,     0,   165,     0,     0,
      51,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      61,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92
};

static const yytype_int16 yycheck[] =
{
      63,    97,    94,    23,     3,    25,    98,    99,     5,   101,
      68,    13,     6,   105,    13,   112,   113,     3,    20,   111,
      37,    20,    80,     6,    41,    19,    46,    13,     6,    31,
      12,    30,    31,     0,    20,    55,    19,     3,    37,    59,
     132,    19,    39,    15,    37,    31,    45,    13,    41,    48,
      70,    37,    38,     4,    20,    33,    26,   154,    41,    45,
       3,    11,    48,     3,   156,    31,    16,    17,    44,    45,
      21,    37,    22,    23,    25,    37,   172,    28,    37,    45,
      39,    35,    48,    32,    40,    38,     6,     3,     3,    15,
       3,     7,     8,    43,   157,    46,    47,   160,    14,    19,
      36,    39,    18,    20,    39,    42,    37,    39,     3,    29,
     173,    27,    40,   176,    38,     3,    35,    39,    34,    35,
      32,    39,     5,    37,     3,    10,    37,    37,    36,     4,
      39,    60,    39,    39,    39,    38,    40,    24,    39,    38,
      42,    41,    39,    39,    38,    38,    38,    28,    22,    39,
      80,    42,    38,   144,    -1,   136,    -1,   147,    -1,    -1,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      59,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    12,    50,    51,    52,    15,     0,    26,    51,     3,
       3,    37,    35,     6,    19,    29,    53,    54,    89,    90,
      32,    55,    56,    38,    40,     3,    15,    57,    58,    55,
      33,    90,    91,    54,    41,    90,    93,     3,    36,    57,
      39,    20,     5,    39,    39,    59,    42,    13,    20,    31,
      92,    94,    37,    90,    39,     3,    61,    62,    90,    38,
      40,    91,    62,    60,    35,    63,    64,    32,    65,    66,
       3,     3,     7,     8,    14,    18,    27,    34,    63,    67,
      68,    69,    70,    71,    75,    76,    77,    78,    79,    80,
      65,    90,    93,    37,    41,    39,    39,    37,    37,    37,
      39,    37,    36,    67,    39,     5,    39,    39,    39,     3,
      30,    37,    45,    48,    71,    72,    73,    74,    81,    82,
      84,    86,    88,    94,    81,     3,    69,    81,    38,    81,
      81,    81,    41,    81,    88,    88,    40,    38,    11,    16,
      17,    22,    23,    43,    83,    44,    45,    85,     4,    21,
      25,    28,    46,    47,    87,    42,    39,    38,    39,    38,
      38,    81,    38,    73,    84,    86,    88,    81,    63,    39,
      63,    42,    39,    10,    69,    63,    38,    63
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    49,    50,    51,    51,    52,    53,    53,    54,    54,
      55,    55,    56,    56,    56,    57,    57,    59,    60,    58,
      61,    61,    62,    62,    64,    63,    65,    65,    66,    66,
      67,    67,    68,    68,    68,    68,    68,    68,    68,    68,
      68,    69,    70,    70,    71,    72,    72,    73,    73,    74,
      74,    75,    75,    76,    77,    78,    78,    78,    79,    80,
      81,    82,    82,    83,    83,    83,    83,    83,    83,    84,
      84,    85,    85,    86,    86,    87,    87,    87,    87,    87,
      87,    88,    88,    88,    88,    88,    88,    88,    89,    89,
      90,    90,    91,    91,    92,    92,    93,    94,    94
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     7,     0,     2,     8,     0,     1,     1,     3,
       0,     2,     4,     6,     4,     0,     2,     0,     0,     9,
       0,     1,     2,     4,     0,     5,     0,     2,     4,     4,
       0,     2,     1,     2,     2,     1,     1,     1,     1,     1,
       1,     3,     1,     4,     4,     1,     1,     0,     1,     1,
       3,     7,     5,     5,     9,     5,     4,     2,     2,     2,
       1,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     1,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     1,     3,     2,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
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
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
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
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
            /* Fall through.  */
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

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
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
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

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
      int yyn = yypact[*yyssp];
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
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
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
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
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
          yyp++;
          yyformat++;
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
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

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
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
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
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
| yyreduce -- Do a reduction.  |
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
#line 111 "lab9.y" /* yacc.c:1646  */
    { 
           
           Program = ASTCreateNode(A_PROGRAM); // Creates A_PROGRAM Node
           Program -> S1 = (yyvsp[-6].astnode); // Externs is S1 connencted 
           if (Search((yyvsp[-4].string), LEVEL,0) != NULL)
                {
                  yyerror("Package has been defined already ");
                  yyerror((yyvsp[-4].string));
                  exit(1);
                }

          
           
           Program -> S2 = ASTCreateNode(A_PACKAGE); // S2 creates A_PACKAGE.
          Program -> S2 -> name = (yyvsp[-4].string); // T_ID will be store in name of S2 .
          Program -> S2 -> S1 = (yyvsp[-2].astnode); // S1 will connect FieldDecls in from S2.
          Program -> S2 -> S2 = (yyvsp[-1].astnode); // S2 will connect MethodDecls in from S2. 
          Program -> S2 -> symbol = Insert ((yyvsp[-4].string),A_Decaf_INT ,ID_Sub_Type_Package,LEVEL,0,0,NULL); // Inserts into the symbol table   

          }
#line 1475 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 135 "lab9.y" /* yacc.c:1646  */
    { 
          (yyval.astnode) = NULL; // set to null if empty
         }
#line 1483 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 139 "lab9.y" /* yacc.c:1646  */
    {
            (yyvsp[-1].astnode) -> next = (yyvsp[0].astnode); // Since there can be more than more we use a list and there for it can be next connencted.
          }
#line 1491 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 146 "lab9.y" /* yacc.c:1646  */
    {
              // check to  make sure extern has been defined 
               if (Search((yyvsp[-5].string), LEVEL,0) != NULL)
                {
                  yyerror("Extern has been defined already ");
                  yyerror((yyvsp[-5].string));
                  exit(1);
                }

               (yyval.astnode) = ASTCreateNode(A_EXTERN); // Creates A_EXTERN Node
               (yyval.astnode) -> name = (yyvsp[-5].string); // T_ID will be store in name of A_EXTERN .
               (yyval.astnode) -> S1 = (yyvsp[-3].astnode); // ExternTypeList will be S1 connected 
               (yyval.astnode) -> A_Declared_Type = (yyvsp[-1].asttype); // The declared type which are The Decaf types INT BOOL VOID and STRING . 
               (yyval.astnode) -> symbol = Insert ((yyvsp[-5].string),(yyvsp[-1].asttype),ID_Sub_Type_Extern,LEVEL,0,0,(yyvsp[-3].astnode)); // Inserts into the symbol table
               
                
                
              
             }
#line 1515 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 167 "lab9.y" /* yacc.c:1646  */
    {
                (yyval.astnode) = NULL; // if the list is empty
                }
#line 1523 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 170 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) = (yyvsp[0].astnode);}
#line 1529 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 173 "lab9.y" /* yacc.c:1646  */
    {
                  (yyval.astnode) = ASTCreateNode(A_EXTERN_TYPE) ; // Creates A_EXTERN_ TYPE Node
                  (yyval.astnode) -> A_Declared_Type = (yyvsp[0].asttype); // The Decaf Type in the Parameters in the Extern Def
                  }
#line 1538 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 178 "lab9.y" /* yacc.c:1646  */
    {
                    (yyval.astnode) = ASTCreateNode(A_EXTERN_TYPE ); // Creates A_EXTERN_ TYPE Node
                    (yyval.astnode) -> A_Declared_Type = (yyvsp[-2].asttype); // // The Decaf Type in the Parameters in the Extern Def Decaf types are  INT BOOL VOID and STRING . 
                    (yyval.astnode) -> next = (yyvsp[0].astnode); // Since there can be more than more we use a list and there for it can be next connencted.
                    }
#line 1548 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 187 "lab9.y" /* yacc.c:1646  */
    {
             (yyval.astnode) = NULL; 
             }
#line 1556 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 192 "lab9.y" /* yacc.c:1646  */
    { // if one or more field delecarations 
              (yyval.astnode) = (yyvsp[-1].astnode); // first field decl
              (yyval.astnode) -> next = (yyvsp[0].astnode); // next connect if more than one 
            
            }
#line 1566 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 201 "lab9.y" /* yacc.c:1646  */
    { 
              // check if the dec is the symtable
               if (Search((yyvsp[-2].string),LEVEL,0) != NULL )
                  {yyerror("Variable has been defined");
                  yyerror((yyvsp[-2].string));
                  exit(1);
                  }
               (yyval.astnode) = ASTCreateNode(A_VARDEC); // creates a node for A_VARDEC  
               (yyval.astnode) -> name = (yyvsp[-2].string); // name gets $2 
               (yyval.astnode) -> A_Declared_Type = (yyvsp[-1].asttype); // declared type INT STRING BOOL or VOID will be $3  
               (yyval.astnode) -> symbol = Insert((yyvsp[-2].string),(yyvsp[-1].asttype), ID_Sub_Type_Scalar, LEVEL, 1,offset,NULL); // Inserts into the symbol table
               offset += 1; // increment offset by 1 
               
             }
#line 1585 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 216 "lab9.y" /* yacc.c:1646  */
    {
              // check to make it has been declared  
              if (Search ((yyvsp[-4].string),LEVEL, 0) != NULL)
              {
                  yyerror ("Variable has been defined");
                  yyerror ((yyvsp[-4].string));
                  exit (1);
              }
             (yyval.astnode) = ASTCreateNode(A_VARDEC);// creates a node for A_VARDEC  
             (yyval.astnode) -> name = (yyvsp[-4].string); // name gets $ 2 
             (yyval.astnode) -> A_Declared_Type = (yyvsp[-3].asttype); // declared type INT STRING BOOL or VOID will be $3
             (yyval.astnode) -> S2 = (yyvsp[-1].astnode); // intial value that is being assigned  
             (yyval.astnode) -> size = 1; // since it is a scalar assign size to 1.
             (yyval.astnode) -> symbol = Insert((yyvsp[-4].string),(yyvsp[-3].asttype), ID_Sub_Type_Scalar, LEVEL, 1,offset,NULL); // Inserts into the symbol table
             offset += 1; // increment offset by 1 
             }
#line 1606 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 234 "lab9.y" /* yacc.c:1646  */
    {
              // checks if array is in the SymbTab
              if (Search((yyvsp[-2].string),LEVEL,0) != NULL)
              {
                    yyerror("Variable for array has been defined ");
                    yyerror((yyvsp[-2].string));
                    exit(1);
               }
              (yyval.astnode) = ASTCreateNode(A_VARDEC); // creates a node for A_VARDEC 
              (yyval.astnode) -> name = (yyvsp[-2].string); // name gets $2 
             (yyval.astnode) -> S1 = (yyvsp[-1].astnode); // ArrayType gets connected to S1.
             (yyval.astnode) -> A_Declared_Type = (yyvsp[-1].astnode) -> A_Declared_Type; // The declared type is whatever the ArrayType will be 
             (yyval.astnode) -> size =  (yyvsp[-1].astnode) -> size; // added lab7set the size what  of T_INTCONSTANT in ArrayType is 
             (yyval.astnode) -> symbol = Insert((yyvsp[-2].string),(yyvsp[-1].astnode) -> A_Declared_Type, ID_Sub_Type_Array, LEVEL,(yyval.astnode) -> size,offset,NULL); // Inserts into the symbol table
             offset += (yyvsp[-1].astnode) -> size;
            
             }
#line 1628 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 255 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) = NULL;}
#line 1634 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 259 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) = (yyvsp[-1].astnode); // first methoddecl
              (yyval.astnode)-> next = (yyvsp[0].astnode); // if more than one next connect them 
              }
#line 1642 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 265 "lab9.y" /* yacc.c:1646  */
    { goffset = offset; offset = 0; maxoffset=offset;}
#line 1648 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 266 "lab9.y" /* yacc.c:1646  */
    {
              // check if methoddecl is in the symtable
               if (Search((yyvsp[-5].string), LEVEL,0) != NULL)
               {
                  yyerror ("Method name has been used");
                  yyerror((yyvsp[-5].string));
                  exit(1);
               } 
                
                // insert into the symtable
                Insert((yyvsp[-5].string),(yyvsp[0].asttype), ID_Sub_Type_Method, LEVEL, 0,0, (yyvsp[-2].astnode));
               
                 maxoffset = offset;
            
                
               }
#line 1669 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 285 "lab9.y" /* yacc.c:1646  */
    { 
               
              (yyval.astnode) = ASTCreateNode (A_METHODDEC); // Creates a A_METHODDEC node 
              (yyval.astnode) -> name = (yyvsp[-7].string);// $2 becomes the name 
              (yyval.astnode) -> A_Declared_Type = (yyvsp[-2].asttype); // MethodType  will be its declared type a MethodType inclueds INT BOOL and VOID
              (yyval.astnode) -> S1 = (yyvsp[-4].astnode); // IndentList will be S1 conneceted 
              (yyval.astnode) -> S2 = (yyvsp[0].astnode); // Block will be S2 connected 
              (yyval.astnode) -> symbol = Search((yyvsp[-7].string), LEVEL,0); // insterts a symbol 
              (yyval.astnode) -> size = maxoffset; // set the size to max offset 
              
              
              //maxoffset = offset-1;
              (yyval.astnode) -> symbol -> mysize = maxoffset; // max size needed 
                
                offset = goffset;
             }
#line 1690 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 303 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) = NULL;}
#line 1696 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 304 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) = (yyvsp[0].astnode);}
#line 1702 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 307 "lab9.y" /* yacc.c:1646  */
    {
            // Checks if method is in the SymbTab
           if(Search((yyvsp[-1].string),LEVEL + 1, 0) != NULL)
            {
                yyerror("duplicate method arg name"); 
                yyerror((yyvsp[-1].string));
                exit(1);
            }
           (yyval.astnode) = ASTCreateNode (A_METHODIDENTIFER  ); // Creates A_METHODIDENTIFER node 
            (yyval.astnode) -> name = (yyvsp[-1].string); // The name will be $1.  
            (yyval.astnode) -> A_Declared_Type = (yyvsp[0].asttype); // $2 becomes the declared type The Type will be INT or BOOL
            (yyval.astnode) -> symbol = Insert((yyvsp[-1].string),(yyvsp[0].asttype), ID_Sub_Type_Scalar, LEVEL + 1, 1,offset,NULL) ; // Inserts into the symbol table
              offset += 1; // increment offset
            }
#line 1721 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 322 "lab9.y" /* yacc.c:1646  */
    { 
              // check if method arg is in the symtable
              if(Search((yyvsp[-3].string),LEVEL + 1, 0) != NULL)
               {
                  yyerror("duplicate method arg name"); 
                  yyerror((yyvsp[-3].string));
                  exit(1);
                } //end if Search
              
              (yyval.astnode) = ASTCreateNode (A_METHODIDENTIFER); // Creates A_METHODIDENTIFER node 
               (yyval.astnode) -> name = (yyvsp[-3].string);// The name will be $1 
               (yyval.astnode) -> A_Declared_Type = (yyvsp[-2].asttype); // $2 becomes the declared type. The Type will be INT or BOOL
               (yyval.astnode) -> size = 1; // set scalar to size 1 
               (yyval.astnode) -> next = (yyvsp[0].astnode); // Since this a list we next connect any other possible IdenList
               (yyval.astnode) -> symbol = Insert ((yyvsp[-3].string),(yyvsp[-2].asttype), ID_Sub_Type_Scalar, LEVEL + 1, 1,offset,NULL) ; // insterts a symbol 
               offset += 1; // increment offset 
            }
#line 1743 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 343 "lab9.y" /* yacc.c:1646  */
    {LEVEL++;}
#line 1749 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 344 "lab9.y" /* yacc.c:1646  */
    {
            (yyval.astnode) = ASTCreateNode(A_BLOCK); // Creates a A_BLOCK node 
            (yyval.astnode) -> S1 = (yyvsp[-2].astnode) ;    /* VarDecls are S1 connected since we added level++  $$ -> S1 = $3 instead of $2 */
            (yyval.astnode) -> S2 = (yyvsp[-1].astnode); // Statements are S2 connected since we added level++  $$ -> S1 = $4 instead of $3
            if(mydebug )Display(); // display shows up when in debugging mode 
            // get the maxium size for maxoffset 
            if (offset > maxoffset)
            {
              maxoffset = offset;
            }
           offset -= Delete(LEVEL); // delete before exiting the block 
            LEVEL --; // decrement level when block is finished 
           }
#line 1767 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 359 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) = NULL;}
#line 1773 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 362 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) = (yyvsp[-1].astnode); // once var decl
         (yyval.astnode) -> next = (yyvsp[0].astnode); // next connected if there are one more VarDecls
         }
#line 1781 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 370 "lab9.y" /* yacc.c:1646  */
    {
              // check if the VarDecl is the symtable video is at 0 had orignially at one 
                if (Search((yyvsp[-2].string),LEVEL,0) != NULL){
                    yyerror("Variable is already defined ");
                    yyerror((yyvsp[-2].string));
                    exit(1);
                   
             }
                 
            (yyval.astnode) = ASTCreateNode(A_VARDEC); // creates a A_VARDEC node
            (yyval.astnode) -> name = (yyvsp[-2].string); // The T_ID is stored in name 
            (yyval.astnode) -> A_Declared_Type = (yyvsp[-1].asttype);// Gets the type which can be INT or BOOL 
            // inserts the VarDecl in the symtable 
            (yyval.astnode) -> symbol = Insert((yyvsp[-2].string),(yyvsp[-1].asttype), ID_Sub_Type_Scalar, LEVEL, 1,offset,NULL); // insterts a symbol 
                 
                offset ++; // increment offset by 1
                
             
          }
#line 1805 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 391 "lab9.y" /* yacc.c:1646  */
    {
           if (Search((yyvsp[-2].string),LEVEL,0) != NULL)
            {
                    yyerror("Variable for array has been defined ");
                    yyerror((yyvsp[-2].string));
                    exit(1);
              }      
           (yyval.astnode) = ASTCreateNode(A_VARDEC); // creates a A_VARDEC node
           (yyval.astnode) -> name = (yyvsp[-2].string); // The T_ID is stored in name 
           (yyval.astnode) -> S1 = (yyvsp[-1].astnode); // The ArrayType is S1 connected 
           (yyval.astnode) -> A_Declared_Type = (yyvsp[-1].astnode) -> A_Declared_Type;  // Gets the type which can be INT or BOOL
           (yyval.astnode) -> size = (yyvsp[-1].astnode) -> size; // size of the array 

           (yyval.astnode) -> symbol = Insert((yyvsp[-2].string),(yyvsp[-1].astnode) ->A_Declared_Type, ID_Sub_Type_Array, LEVEL, (yyval.astnode)->size ,offset,NULL); // insterts a symbol 
         
           offset += (yyvsp[-1].astnode) -> size; // increment offset by array size
          }
#line 1827 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 413 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) = NULL;}
#line 1833 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 416 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) =(yyvsp[-1].astnode); 
               (yyval.astnode) -> next = (yyvsp[0].astnode);}
#line 1840 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 423 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) = (yyvsp[0].astnode);}
#line 1846 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 424 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) = (yyvsp[-1].astnode);}
#line 1852 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 425 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) = (yyvsp[-1].astnode);}
#line 1858 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 426 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) = (yyvsp[0].astnode);}
#line 1864 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 427 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) = (yyvsp[0].astnode);}
#line 1870 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 428 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) = (yyvsp[0].astnode) ;}
#line 1876 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 429 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) = (yyvsp[0].astnode);}
#line 1882 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 430 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) = (yyvsp[0].astnode);}
#line 1888 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 431 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) = (yyvsp[0].astnode);}
#line 1894 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 438 "lab9.y" /* yacc.c:1646  */
    { 
            // make sure lvalue and expression have the same type 
            if ((yyvsp[-2].astnode) -> A_Declared_Type != (yyvsp[0].astnode) -> A_Declared_Type)
              {
                yyerror("type mismatch on assignment"); 
                exit(1);
              }
              (yyval.astnode) = ASTCreateNode (A_ASSIGN); // Creates a A_ASSIGN  node
              (yyval.astnode) -> S1 = (yyvsp[-2].astnode);// Lvalue is S1 connected 
              (yyval.astnode) -> S2 = (yyvsp[0].astnode); // Expr is S2 connected 
              (yyval.astnode) -> name = Create_Temp(); // assigns name with Create_Temp
              
              (yyval.astnode) -> symbol = Insert ((yyval.astnode) -> name, (yyval.astnode) -> A_Declared_Type,ID_Sub_Type_Scalar,LEVEL,1,offset,NULL); // insert symbol
              offset++; // increments offset 
              }
#line 1914 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 455 "lab9.y" /* yacc.c:1646  */
    { 
             struct SymbTab *p ;
             p = Search((yyvsp[0].string),LEVEL,1); // looking to see if the variable has been declared 

             // Barfs if not in the symtable 
             if (p == NULL)
             {
              yyerror("Lvalue hasnt been defined ");
              yyerror ((yyvsp[0].string));
              exit(1);
             }
             // barfs if it is not a scalar 
             if (p -> SubType != ID_Sub_Type_Scalar)
             {
              yyerror((yyvsp[0].string));
              yyerror("Wrong SubType , needs to be a Scalar");
              exit(1);
             }
            
            (yyval.astnode) = ASTCreateNode(A_VAR_LVALUE); // Creates a A_VAR_LVALUE
            (yyval.astnode) -> name = (yyvsp[0].string); // T_ID is stored in  name 
            (yyval.astnode) -> symbol = p; // assigns symbol from p 
            (yyval.astnode) -> A_Declared_Type = p -> Type; // assigns declared p type 
           }
#line 1943 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 481 "lab9.y" /* yacc.c:1646  */
    { struct SymbTab *p;
             p = Search((yyvsp[-3].string),LEVEL,1); // looking to see if the variable has been declared 
             
             // barfs if not in the SymbTab
             if (p == NULL)
             {
              yyerror("Lvalue array hasnt been defined ");
              yyerror ((yyvsp[-3].string));
              exit(1);
             }
             // barfs if not array type 
             if (p -> SubType != ID_Sub_Type_Array)
             {
              yyerror((yyvsp[-3].string));
              yyerror("Wrong SubType , needs to be am array");
              exit(1);
             }
            (yyval.astnode) = ASTCreateNode(A_VAR_LVALUE); // Creates A_VAR_LVALUE node
            (yyval.astnode) -> name = (yyvsp[-3].string); // T_ID is store in name 
            (yyval.astnode) -> S1 = (yyvsp[-1].astnode); // Expr is S1 connected
            (yyval.astnode) -> symbol = p;// assigns symbol from p 

            (yyval.astnode) -> A_Declared_Type = p -> Type; // assigns declared p type 
            }
#line 1972 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 509 "lab9.y" /* yacc.c:1646  */
    {
               struct SymbTab *p; 
             p = Search((yyvsp[-3].string),LEVEL,1); // looking to see if the method has been declared 
             // barfs if method has not been declared 
             if (p == NULL)
             {
              yyerror("Method hasnt been defined ");
              yyerror ((yyvsp[-3].string));
              exit(1);
             }
             // barfs if the subType is not a method or extern method  
             if ((p -> SubType != ID_Sub_Type_Method) && (p -> SubType != ID_Sub_Type_Extern))
             
             {
              yyerror((yyvsp[-3].string));
              yyerror("Wrong SubType, needs to be a Method");
              exit(1);
             }
              (yyval.astnode) = ASTCreateNode(A_METHODCALL); // Creates a A_METHODCALL node 
              (yyval.astnode) -> name = (yyvsp[-3].string); // T_ID is store in name 
              (yyval.astnode) ->symbol = p; // set the symbol 
              (yyval.astnode) -> S1 = (yyvsp[-1].astnode);
              (yyval.astnode) -> A_Declared_Type = p -> Type;
            // we need to check the formal parameters' length and type is the same as the actual parameters
             if (check_parameters ( p->fparms , (yyvsp[-1].astnode)) == 0 )
             {
              yyerror ("Formal  and actual paramters  do not match");
              yyerror ((yyvsp[-1].astnode));
              printf("p -> fparms = %d $3 = %d \n", p -> fparms ->A_Declared_Type, (yyvsp[-1].astnode) -> A_Declared_Type);
              exit(1);
             }// end check_parameters
              }
#line 2009 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 545 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) = ASTCreateNode(A_METHOD_ARG);
              (yyval.astnode) -> S1 = (yyvsp[0].astnode); // Creates a Method Arg node and connects S1 Expr 
              (yyval.astnode) ->  A_Declared_Type = (yyvsp[0].astnode) -> A_Declared_Type;
              (yyval.astnode) -> name = Create_Temp(); // assigns name with Create_Temp
              (yyval.astnode) -> symbol = Insert ((yyval.astnode) -> name, (yyval.astnode) -> A_Declared_Type,ID_Sub_Type_Scalar,LEVEL,1,offset,NULL); // inserts symbol in symtab
               offset ++; // increment offset 
              
             }
#line 2022 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 555 "lab9.y" /* yacc.c:1646  */
    {
              (yyval.astnode) = ASTCreateNode (A_METHOD_ARG );
              (yyval.astnode) -> symbol = Insert (Create_Temp(), A_Decaf_STRING,ID_Sub_Type_String ,LEVEL,1,offset++,NULL); // inserts symbol in symtab
              (yyval.astnode) ->  A_Declared_Type = A_Decaf_STRING;
              (yyval.astnode) -> S1 = ASTCreateNode(A_CONTSTANT_STRING); //Creates A_CONTSTANT_STRING node 
              (yyval.astnode) -> S1 ->  A_Declared_Type = A_Decaf_STRING; // Assign to String 
               (yyval.astnode) -> S1 -> name = (yyvsp[0].string);
             
              
              
             }
#line 2038 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 571 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) = NULL;}
#line 2044 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 572 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) = (yyvsp[0].astnode);}
#line 2050 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 576 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) = (yyvsp[0].astnode);}
#line 2056 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 577 "lab9.y" /* yacc.c:1646  */
    { // if more than one  argument
                  (yyval.astnode) = (yyvsp[-2].astnode); // first argument wil get $1 
                  (yyval.astnode) -> next = (yyvsp[0].astnode); // since this is a list it will be next connected for two or more arguments.
                  //$$ -> name = Create_Temp(); // assigns name with Create_Temp
                  //$$ -> symbol = Insert ($$ -> name, $$ -> A_Declared_Type,ID_Sub_Type_Scalar,LEVEL,1,offset,NULL); // inserts symbol in symtab
                  //offset++;
                  
                 }
#line 2069 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 587 "lab9.y" /* yacc.c:1646  */
    {
              (yyval.astnode) = ASTCreateNode(A_IFSTMT);// Creates a A_IFSTMT node 
              (yyval.astnode) -> S1 = (yyvsp[-4].astnode); //  Expr is S1 connected 
              (yyval.astnode) -> S2 = ASTCreateNode(A_IFBODY); // Creates A_IFBODY node that will be S2 connected 
              (yyval.astnode) -> S2 -> S1 = (yyvsp[-2].astnode); // Since A_IFBODY is S2 connected IFBODY S1 will be Block 
              (yyval.astnode) -> S2 -> S2 = (yyvsp[0].astnode); // removed A_ELSE node unnessary node T_ELSE is S2 connected to IFBody 
              
              
              
             }
#line 2084 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 598 "lab9.y" /* yacc.c:1646  */
    {
              (yyval.astnode) = ASTCreateNode(A_IFSTMT); // Creates a A_IFSTMT node 
              (yyval.astnode) -> S1 = (yyvsp[-2].astnode); //  Expr is S1 connected
              (yyval.astnode) -> S2 = ASTCreateNode(A_IFBODY); // Creates A_IFBODY node that will be S2 connected
              (yyval.astnode) -> S2 -> S1 = (yyvsp[0].astnode); // Since A_IFBODY is S2 connected IFBODY S1 will be Block 
              (yyval.astnode) -> S2 -> S2 = NULL; // Since this doesnt contain a Else it will remain null 
              }
#line 2096 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 609 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) = ASTCreateNode(A_WHILESTMT); // Creates a A_WHILESTMT node 
                   (yyval.astnode) -> S1 = (yyvsp[-2].astnode); // Expr is S1 connected 
                   (yyval.astnode) -> S2 = (yyvsp[0].astnode);// Block is S2 connected 
                   }
#line 2105 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 617 "lab9.y" /* yacc.c:1646  */
    {
                  (yyval.astnode) = ASTCreateNode(A_FORSTMT); // creates a A_FORSTMT node 
                  (yyval.astnode) -> S1 = (yyvsp[-6].astnode); // Assign  is S1 connected 
                  (yyval.astnode) -> S2 = (yyvsp[-4].astnode); // Exp is S2
                  (yyval.astnode)-> S1 -> S1 = (yyvsp[-2].astnode); // Assign is S1 connected from S1
                  (yyval.astnode)-> S1 -> S2 = (yyvsp[0].astnode); // Block is S2 connected from S1
                }
#line 2117 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 627 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) = ASTCreateNode(A_RETURN); // Creates a A_RETURN node 
              (yyval.astnode) ->S1 = (yyvsp[-2].astnode); // Expr is S1 connected 
              }
#line 2125 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 631 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) = ASTCreateNode(A_RETURN);}
#line 2131 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 633 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) = ASTCreateNode(A_RETURN);}
#line 2137 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 638 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = ASTCreateNode(A_BREAK); // Creates a A_BREAK node 
           }
#line 2144 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 644 "lab9.y" /* yacc.c:1646  */
    {
             (yyval.astnode) = ASTCreateNode(A_CONTINUE); // Creates a A_CONTINUE node 
           }
#line 2152 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 649 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) = (yyvsp[0].astnode) ; }
#line 2158 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 653 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) = (yyvsp[0].astnode) ; }
#line 2164 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 655 "lab9.y" /* yacc.c:1646  */
    { if((yyvsp[-2].astnode) -> A_Declared_Type != (yyvsp[0].astnode) -> A_Declared_Type)
                      {
                          yyerror("Both sides must be the same type");
                          exit(1);
                      }
                    
                    (yyval.astnode) = ASTCreateNode (A_EXPR); // Creates a A_EXPR node 
                   (yyval.astnode) -> S1 = (yyvsp[-2].astnode); // Expr S1 is assigned to $1 
                   (yyval.astnode) -> operator = (yyvsp[-1].operator); // opreator gets $2 
                   (yyval.astnode) -> A_Declared_Type = A_Decaf_BOOL; // boolean
                   (yyval.astnode) -> S2 = (yyvsp[0].astnode); //S2 is gets $3
                   (yyval.astnode) -> name = Create_Temp(); // assigns name with Create_Temp
                   (yyval.astnode) -> symbol = Insert ((yyval.astnode) -> name, (yyval.astnode) -> A_Declared_Type,ID_Sub_Type_Scalar,LEVEL,1,offset,NULL); // assigns name with Create_Temp
                   offset ++; // increment offset

                  }
#line 2185 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 674 "lab9.y" /* yacc.c:1646  */
    {(yyval.operator) = A_LEQ;}
#line 2191 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 675 "lab9.y" /* yacc.c:1646  */
    {(yyval.operator) = A_LT;}
#line 2197 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 676 "lab9.y" /* yacc.c:1646  */
    {(yyval.operator) = A_GT;}
#line 2203 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 677 "lab9.y" /* yacc.c:1646  */
    {(yyval.operator) = A_GEQ;}
#line 2209 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 678 "lab9.y" /* yacc.c:1646  */
    {(yyval.operator) = A_EQ;}
#line 2215 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 679 "lab9.y" /* yacc.c:1646  */
    {(yyval.operator) = A_NEQ;}
#line 2221 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 682 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) = (yyvsp[0].astnode) ; }
#line 2227 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 684 "lab9.y" /* yacc.c:1646  */
    { if ((yyvsp[-2].astnode) -> A_Declared_Type != (yyvsp[0].astnode) -> A_Declared_Type || (yyvsp[-2].astnode) -> A_Declared_Type != A_Decaf_INT)
         {
           yyerror ("Additon and Subtraction need integers only");
           exit(1);

         }
        (yyval.astnode) = ASTCreateNode (A_EXPR); // Creates a A_EXPR node 
      (yyval.astnode) -> S1 = (yyvsp[-2].astnode); // S1 is assigned with $1 
      (yyval.astnode) -> operator = (yyvsp[-1].operator); // operator is assigned with $2 
      (yyval.astnode) -> S2 = (yyvsp[0].astnode); // S2 is assigned with $3 
      (yyval.astnode) -> A_Declared_Type = (yyvsp[-2].astnode) -> A_Declared_Type;
      (yyval.astnode) -> name = Create_Temp(); // assigns name to  Create_Temp to store temporary value in symtab
      (yyval.astnode) -> symbol = Insert ((yyval.astnode) -> name, (yyval.astnode) -> A_Declared_Type,ID_Sub_Type_Scalar,LEVEL,1,offset,NULL); // inserts symbol in a symtable 
            offset ++; // increment offset

      }
#line 2248 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 702 "lab9.y" /* yacc.c:1646  */
    {(yyval.operator) = A_PLUS;}
#line 2254 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 703 "lab9.y" /* yacc.c:1646  */
    {(yyval.operator) = A_MINUS;}
#line 2260 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 706 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) = (yyvsp[0].astnode) ; }
#line 2266 "y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 708 "lab9.y" /* yacc.c:1646  */
    {  if ((yyvsp[-2].astnode) ->  A_Declared_Type != (yyvsp[0].astnode) -> A_Declared_Type  )
          {
                yyerror ("type mismatch"); 
                exit(1);
          }
          if (((yyvsp[-2].astnode) -> A_Declared_Type == A_Decaf_BOOL) && ((yyvsp[-1].operator) == A_TIMES || (yyvsp[-1].operator) == A_DIVIDE))
          {
            yyerror("Cant use booleans on a arithemetic operation " ); 
            exit(1);
          }

         if (((yyvsp[-2].astnode) -> A_Declared_Type == A_Decaf_INT) && ((yyvsp[-1].operator) == A_AND || (yyvsp[-1].operator) == A_OR || (yyvsp[-1].operator) == A_LEFTSHIFT || (yyvsp[-1].operator) == A_RIGHTSHIFT ))
          {
            yyerror("Cant use  a boolean operator on integers" ); 
            exit(1);
          } 
        (yyval.astnode) = ASTCreateNode (A_EXPR); // Creates a A_EXPR node 
      (yyval.astnode) -> S1 = (yyvsp[-2].astnode); // S1 is assigned $1 
      (yyval.astnode) -> operator = (yyvsp[-1].operator); // operator is assigned $2 
      (yyval.astnode) -> A_Declared_Type = (yyvsp[-2].astnode) -> A_Declared_Type;
      (yyval.astnode) -> S2 = (yyvsp[0].astnode); //S2 is assigned with $3
      
      (yyval.astnode) -> name = Create_Temp(); // assigns name with Create_Temp
      (yyval.astnode) -> symbol = Insert ((yyval.astnode) -> name, (yyval.astnode) -> A_Declared_Type,ID_Sub_Type_Scalar,LEVEL,1,offset,NULL); // assigns name with Create_Temp
        offset ++; // increment offset 

      
      }
#line 2299 "y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 739 "lab9.y" /* yacc.c:1646  */
    {(yyval.operator) = A_TIMES ; }
#line 2305 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 740 "lab9.y" /* yacc.c:1646  */
    {(yyval.operator) = A_DIVIDE ; }
#line 2311 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 741 "lab9.y" /* yacc.c:1646  */
    {(yyval.operator) = A_AND ; }
#line 2317 "y.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 742 "lab9.y" /* yacc.c:1646  */
    {(yyval.operator) = A_OR ;}
#line 2323 "y.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 743 "lab9.y" /* yacc.c:1646  */
    {(yyval.operator) = A_LEFTSHIFT ;}
#line 2329 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 744 "lab9.y" /* yacc.c:1646  */
    {(yyval.operator) = A_RIGHTSHIFT ;}
#line 2335 "y.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 748 "lab9.y" /* yacc.c:1646  */
    {
          struct SymbTab *p;
             p = Search((yyvsp[0].string),LEVEL,1); // looking to see if the variable has been declared 
             // Barfs if Rvalue is not in the SymbTab
             if (p == NULL)
             {
              yyerror("Rvalue hasnt been defined ");
              yyerror ((yyvsp[0].string));
              exit(1);
             }
             // Barfs if subType not a scalar
             if (p -> SubType != ID_Sub_Type_Scalar)
             {
              yyerror((yyvsp[0].string));
              yyerror("Wrong SubType , needs to be a Scalar");
              exit(1);
             }
          (yyval.astnode) = ASTCreateNode(A_VAR_RVALUE) ; // Creates a A_VAR_RVALUE  
          (yyval.astnode) -> name = (yyvsp[0].string); // T_ID is store in name
          (yyval.astnode) -> symbol = p; // assigns symbol from p 
          (yyval.astnode) -> A_Declared_Type = p -> Type;
          }
#line 2362 "y.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 771 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) = (yyvsp[0].astnode);}
#line 2368 "y.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 773 "lab9.y" /* yacc.c:1646  */
    {
          struct SymbTab *p ;
             p = Search((yyvsp[-3].string),LEVEL,1); // looking to see if the variable has been declared 
              // barfs if array is not in the SymbTab
             if (p == NULL)
             {
              yyerror("Array hasnt been defined ");
              yyerror ((yyvsp[-3].string));
              exit(1);
             }
            // Barfs if subType is not array
             if (p -> SubType != ID_Sub_Type_Array)
             {
              yyerror((yyvsp[-3].string));
              yyerror("Wrong SubType , needs to be an Array");
              exit(1);
             }
          (yyval.astnode) = ASTCreateNode(A_VAR_RVALUE) ; // Creates a A_VAR_RVALUE  
          (yyval.astnode) -> name = (yyvsp[-3].string); // T_ID is sotre in name 
          (yyval.astnode) -> S1 = (yyvsp[-1].astnode);// S1 is assigned with $3 
          (yyval.astnode) -> symbol = p; // assigns symbol from p 
          (yyval.astnode) -> A_Declared_Type = p -> Type;
        }
#line 2396 "y.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 796 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) = (yyvsp[0].astnode) ; }
#line 2402 "y.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 797 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) = (yyvsp[-1].astnode) ; }
#line 2408 "y.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 798 "lab9.y" /* yacc.c:1646  */
    {
           // Barfs if assignment value is not a boolean value
           if ((yyvsp[0].astnode) ->A_Declared_Type != A_Decaf_BOOL)
             {
              yyerror(" type mismatch, expected a boolean ");
             
              exit(1);
              }

         (yyval.astnode) = ASTCreateNode(A_EXPR); // creates a A_EXPR 
         (yyval.astnode) -> operator = A_NOT; // assigned operator to A_NOT
         (yyval.astnode) -> S1 = (yyvsp[0].astnode); // S1 is assigned to $2 
         (yyval.astnode) -> A_Declared_Type = A_Decaf_BOOL; 
         (yyval.astnode) -> name = Create_Temp(); // assigns name to  Create_Temp to store temporary value in symtab
         (yyval.astnode) -> symbol = Insert ((yyval.astnode) -> name, A_Decaf_BOOL,ID_Sub_Type_Scalar,LEVEL,1,offset,NULL); // Inserts symbol in SymbTab
         offset ++; // increments offset
         }
#line 2430 "y.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 817 "lab9.y" /* yacc.c:1646  */
    {
         (yyval.astnode) = ASTCreateNode(A_EXPR); // creates a A_EXPR node 
         (yyval.astnode) -> operator =  A_MINUS; // operator is assigned A_Minus 
         (yyval.astnode) -> S1 = (yyvsp[0].astnode); // S1 is assigned to $2 
         (yyval.astnode) -> A_Declared_Type = A_Decaf_INT;
         (yyval.astnode) -> name = Create_Temp(); // assigns name to  Create_Temp to get the tempory value of Expr
         (yyval.astnode) -> symbol = Insert ((yyval.astnode) -> name, A_Decaf_INT,ID_Sub_Type_Scalar,LEVEL,1,offset,NULL); // Inserts symbol in SymbTab // change from BOOL to INT
         offset ++;// increments offset
         }
#line 2444 "y.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 829 "lab9.y" /* yacc.c:1646  */
    {(yyval.asttype) = A_Decaf_STRING;}
#line 2450 "y.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 833 "lab9.y" /* yacc.c:1646  */
    {(yyval.asttype) = A_Decaf_INT;}
#line 2456 "y.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 834 "lab9.y" /* yacc.c:1646  */
    {(yyval.asttype) = A_Decaf_BOOL;}
#line 2462 "y.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 837 "lab9.y" /* yacc.c:1646  */
    { (yyval.asttype) = A_Decaf_VOID; }
#line 2468 "y.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 838 "lab9.y" /* yacc.c:1646  */
    { (yyval.asttype) = (yyvsp[0].asttype);}
#line 2474 "y.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 842 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode)  = ASTCreateNode(A_CONTSTANT_BOOL) ; // Creates A_CONTSTANT_BOOL node 
              (yyval.astnode) -> value = 1; // value 1 means it is true 
              (yyval.astnode) -> A_Declared_Type = A_Decaf_BOOL;
              
              }
#line 2484 "y.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 848 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode)  = ASTCreateNode(A_CONTSTANT_BOOL) ; // Creates A_CONTSTANT_BOOL
              (yyval.astnode) -> value = 0;
              (yyval.astnode) -> A_Declared_Type = A_Decaf_BOOL;
              }
#line 2493 "y.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 855 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) = ASTCreateNode(A_ARRAYTYPE); // Creates a Node for A_ARRAYTYPE
          (yyval.astnode) -> value = (yyvsp[-2].value); // whatever is in T_INTCONSTANT value is assigned 
          (yyval.astnode) -> size = (yyvsp[-2].value); // size of the array
          (yyval.astnode) -> A_Declared_Type = (yyvsp[0].asttype); // The declared type can be INT or BOOL 
          }
#line 2503 "y.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 863 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) = ASTCreateNode(A_CONTSTANT_INT); // It can be either a T_INTCONSTANT
              (yyval.astnode) -> value =(yyvsp[0].value);
              (yyval.astnode) -> A_Declared_Type = A_Decaf_INT;
              }
#line 2512 "y.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 867 "lab9.y" /* yacc.c:1646  */
    {(yyval.astnode) = (yyvsp[0].astnode); 
                          // OR it can BOOL constant 
               }
#line 2520 "y.tab.c" /* yacc.c:1646  */
    break;


#line 2524 "y.tab.c" /* yacc.c:1646  */
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

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

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
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

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
                  yystos[*yyssp], yyvsp);
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
#line 871 "lab9.y" /* yacc.c:1906  */
	/* end of rules, start of program */

// We assume the input is correct without errors
int main(int argc, char * argv[])
{   
   int i = 1; 
   FILE *fp; 
   char s [100];
    // processes the input line 
    while (i < argc)
    {
       
      if (strcmp (argv[i], "-d") == 0 )
      { // we have a debug 
        mydebug = 1; 
      }

      if (strcmp (argv[i], "-o") == 0 )
      { // we have a input file 
         // Copy argv into a temporary 
         // add suffix 
         // attempt to open the file

         strcpy (s, argv[ i  + 1 ]); 
         strcat (s, ".s");
         
         
         if ( (fp = fopen(s,"w")) == NULL )
         {
            printf(" %s cannot open \n", argv[i + 1]);
            exit(1);
         }
      }
       i = i + 1;  
    }
    yyparse();
    if(mydebug) Display(); // display shows up when in debugging mode 
    if(mydebug) printf("Parsing Complete \n"); // Prints when Parsing is completed in deugging mode 
     if(mydebug) ASTprint(0, Program); // Prints the AST tree in debugging mode
     EMIT(Program,fp);
}
