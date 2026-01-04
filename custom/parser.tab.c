/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include "types.h"
#include "mips_bin.h"

extern int yylex();
extern int yyparse();
extern FILE *yyin;
extern int yylineno;

void yyerror(const char *s);

// Global variables
VARIABLE *var_head = NULL;
VARIABLE *var_tail = NULL;
ERROR *err_head = NULL;
ERROR *err_tail = NULL;
HISTORY *hist_head = NULL;
HISTORY *hist_tail = NULL;
MIPS_INSTR *mips_head = NULL;
MIPS_INSTR *mips_tail = NULL;
int has_errors = 0;
int line_number = 1;
DataType current_type = TYPE_UNKNOWN;

// Function declarations
void read_output_file(const char* filename);
void create_empty_file(const char* filename);
void log_event(const char *format, ...);
void add_error(char *msg, int line);
void show_errors();
void add_mips_instr(char *code);
VARIABLE *find_variable(char *name);
VARIABLE *add_variable(char *name, DataType type);
int is_valid_identifier(char *str);
ASTNode* create_node(ASTNodeType type, int line);
int eval_ast(ASTNode *node);
void gen_mips_ast(ASTNode *node, int reg);
void generate_mips_file(const char* filename);
void print_results();
void cleanup_all();


#line 119 "parser.tab.c"

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

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_NUMBER = 3,                     /* NUMBER  */
  YYSYMBOL_CHAR_LITERAL = 4,               /* CHAR_LITERAL  */
  YYSYMBOL_IDENTIFIER = 5,                 /* IDENTIFIER  */
  YYSYMBOL_INT = 6,                        /* INT  */
  YYSYMBOL_CHAR = 7,                       /* CHAR  */
  YYSYMBOL_ASSIGN = 8,                     /* ASSIGN  */
  YYSYMBOL_SEMICOLON = 9,                  /* SEMICOLON  */
  YYSYMBOL_COMMA = 10,                     /* COMMA  */
  YYSYMBOL_PLUS = 11,                      /* PLUS  */
  YYSYMBOL_MINUS = 12,                     /* MINUS  */
  YYSYMBOL_MULTIPLY = 13,                  /* MULTIPLY  */
  YYSYMBOL_DIVIDE = 14,                    /* DIVIDE  */
  YYSYMBOL_LPAREN = 15,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 16,                    /* RPAREN  */
  YYSYMBOL_INCREMENT = 17,                 /* INCREMENT  */
  YYSYMBOL_DECREMENT = 18,                 /* DECREMENT  */
  YYSYMBOL_CDISPLAY = 19,                  /* CDISPLAY  */
  YYSYMBOL_UNARY = 20,                     /* UNARY  */
  YYSYMBOL_YYACCEPT = 21,                  /* $accept  */
  YYSYMBOL_program = 22,                   /* program  */
  YYSYMBOL_statement = 23,                 /* statement  */
  YYSYMBOL_type = 24,                      /* type  */
  YYSYMBOL_declaration = 25,               /* declaration  */
  YYSYMBOL_var_list = 26,                  /* var_list  */
  YYSYMBOL_var_decl = 27,                  /* var_decl  */
  YYSYMBOL_assignment = 28,                /* assignment  */
  YYSYMBOL_expression_statement = 29,      /* expression_statement  */
  YYSYMBOL_increment_decrement = 30,       /* increment_decrement  */
  YYSYMBOL_expression = 31,                /* expression  */
  YYSYMBOL_term = 32,                      /* term  */
  YYSYMBOL_factor = 33,                    /* factor  */
  YYSYMBOL_cdisplay_statement = 34         /* cdisplay_statement  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




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

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
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
typedef yytype_int8 yy_state_t;

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
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
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

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

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
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   61

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  21
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  14
/* YYNRULES -- Number of rules.  */
#define YYNRULES  36
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  62

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   275


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    75,    75,    77,    81,    82,    83,    84,    85,    86,
      90,    91,    95,    99,   100,   104,   126,   171,   205,   218,
     243,   268,   293,   321,   322,   331,   343,   344,   353,   365,
     369,   373,   386,   389,   400,   408,   425
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "NUMBER",
  "CHAR_LITERAL", "IDENTIFIER", "INT", "CHAR", "ASSIGN", "SEMICOLON",
  "COMMA", "PLUS", "MINUS", "MULTIPLY", "DIVIDE", "LPAREN", "RPAREN",
  "INCREMENT", "DECREMENT", "CDISPLAY", "UNARY", "$accept", "program",
  "statement", "type", "declaration", "var_list", "var_decl", "assignment",
  "expression_statement", "increment_decrement", "expression", "term",
  "factor", "cdisplay_statement", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-12)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
     -12,    16,   -12,     5,   -12,   -12,    -5,   -12,   -12,    39,
      39,    39,     1,    13,    17,   -12,    24,    27,    30,    31,
      43,    -4,    -3,   -12,    48,   -12,    39,   -12,   -12,   -12,
     -12,   -12,    -7,   -12,   -12,    44,    45,    50,   -12,   -12,
     -12,   -12,   -12,    39,    39,    39,    39,   -12,    -4,   -12,
      42,    14,    39,    24,    -3,    -3,   -12,   -12,   -12,   -12,
      -4,   -12
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     1,     0,    29,    30,    31,    10,    11,     0,
       0,     0,     0,     0,     0,     3,     0,     0,     0,     0,
       0,    18,    23,    26,     0,     9,     0,    19,    20,    31,
      34,    33,     0,    21,    22,     0,    15,    12,    13,     4,
       5,     6,     7,     0,     0,     0,     0,     8,    17,    32,
      31,     0,     0,     0,    24,    25,    27,    28,    36,    35,
      16,    14
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -12,   -12,   -12,   -12,   -12,   -12,     8,   -12,   -12,   -12,
     -11,     2,    -8,   -12
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,    15,    16,    17,    37,    38,    18,    19,    20,
      21,    22,    23,    24
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      32,    30,    31,    26,    43,    44,    33,    43,    44,    49,
      45,    46,    27,    28,    25,    48,     2,     3,    34,     4,
       5,     6,     7,     8,    51,    43,    44,     9,    10,    36,
      59,    11,    35,    12,    13,    14,    39,    56,    57,    40,
      41,    60,     4,     5,    29,    54,    55,     4,     5,    50,
       9,    10,    42,    52,    11,     9,    10,    47,    58,    11,
      53,    61
};

static const yytype_int8 yycheck[] =
{
      11,     9,    10,     8,    11,    12,     5,    11,    12,    16,
      13,    14,    17,    18,     9,    26,     0,     1,     5,     3,
       4,     5,     6,     7,    35,    11,    12,    11,    12,     5,
      16,    15,    15,    17,    18,    19,     9,    45,    46,     9,
       9,    52,     3,     4,     5,    43,    44,     3,     4,     5,
      11,    12,     9,     8,    15,    11,    12,     9,    16,    15,
      10,    53
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    22,     0,     1,     3,     4,     5,     6,     7,    11,
      12,    15,    17,    18,    19,    23,    24,    25,    28,    29,
      30,    31,    32,    33,    34,     9,     8,    17,    18,     5,
      33,    33,    31,     5,     5,    15,     5,    26,    27,     9,
       9,     9,     9,    11,    12,    13,    14,     9,    31,    16,
       5,    31,     8,    10,    32,    32,    33,    33,    16,    16,
      31,    27
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    21,    22,    22,    23,    23,    23,    23,    23,    23,
      24,    24,    25,    26,    26,    27,    27,    28,    29,    30,
      30,    30,    30,    31,    31,    31,    32,    32,    32,    33,
      33,    33,    33,    33,    33,    34,    34
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       1,     1,     2,     1,     3,     1,     3,     3,     1,     2,
       2,     2,     2,     1,     3,     3,     1,     3,     3,     1,
       1,     1,     3,     2,     2,     4,     4
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


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

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


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




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
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
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
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
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
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
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

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
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
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
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  case 9: /* statement: error SEMICOLON  */
#line 86 "parser.y"
                      { yyerrok; }
#line 1174 "parser.tab.c"
    break;

  case 10: /* type: INT  */
#line 90 "parser.y"
            { (yyval.dtype) = TYPE_INT; current_type = TYPE_INT; }
#line 1180 "parser.tab.c"
    break;

  case 11: /* type: CHAR  */
#line 91 "parser.y"
            { (yyval.dtype) = TYPE_CHAR; current_type = TYPE_CHAR; }
#line 1186 "parser.tab.c"
    break;

  case 12: /* declaration: type var_list  */
#line 95 "parser.y"
                    { current_type = TYPE_UNKNOWN; }
#line 1192 "parser.tab.c"
    break;

  case 15: /* var_decl: IDENTIFIER  */
#line 104 "parser.y"
               {
        char log_buffer[256];
        if (!is_valid_identifier((yyvsp[0].str))) {
            char buffer[200];
            sprintf(buffer, "Error: Variable '%s' contains invalid characters!", (yyvsp[0].str));
            add_error(buffer, line_number);
            sprintf(log_buffer, "[Line %d] ERROR: Invalid variable name '%s'", line_number, (yyvsp[0].str));
            log_event(log_buffer);
        } else if (find_variable((yyvsp[0].str)) != NULL) {
            char buffer[200];
            sprintf(buffer, "Error: Variable '%s' is already defined!", (yyvsp[0].str));
            add_error(buffer, line_number);
            sprintf(log_buffer, "[Line %d] ERROR: Failed to declare '%s' (Exists)", line_number, (yyvsp[0].str));
            log_event(log_buffer);
        } else {
            VARIABLE *var = add_variable((yyvsp[0].str), current_type);
            char *t = (current_type == TYPE_INT) ? "INT" : "CHAR";
            sprintf(log_buffer, "[Line %d] DECLARE: Created variable '%s' of type %s", line_number, (yyvsp[0].str), t);
            log_event(log_buffer);
        }
        free((yyvsp[0].str));
    }
#line 1219 "parser.tab.c"
    break;

  case 16: /* var_decl: IDENTIFIER ASSIGN expression  */
#line 126 "parser.y"
                                   {
        char log_buffer[256];
        char mips_buffer[256];
        
        if (!is_valid_identifier((yyvsp[-2].str))) {
            char buffer[200];
            sprintf(buffer, "Error: Variable '%s' contains invalid characters!", (yyvsp[-2].str));
            add_error(buffer, line_number);
        } else {
            VARIABLE *var = find_variable((yyvsp[-2].str));
            if (var != NULL) {
                char buffer[200];
                sprintf(buffer, "Error: Variable '%s' is already defined!", (yyvsp[-2].str));
                add_error(buffer, line_number);
            } else {
                var = add_variable((yyvsp[-2].str), current_type);
                char *t = (current_type == TYPE_INT) ? "INT" : "CHAR";
                sprintf(log_buffer, "[Line %d] DECLARE: Created variable '%s' of type %s", line_number, (yyvsp[-2].str), t);
                log_event(log_buffer);
                
                if ((yyvsp[0].node) != NULL && !(yyvsp[0].node)->has_error) {
                    int old_val = var->value;
                    var->value = eval_ast((yyvsp[0].node));
                    
                    // Log with ASCII character if it's a char type and printable
                    if (current_type == TYPE_CHAR && var->value >= 32 && var->value <= 126) {
                        sprintf(log_buffer, "[Line %d] ASSIGN: Updated '%s' from %d to %d (ASCII: '%c')", 
                                line_number, var->name, old_val, var->value, (char)var->value);
                    } else {
                        sprintf(log_buffer, "[Line %d] ASSIGN: Updated '%s' from %d to %d", 
                                line_number, var->name, old_val, var->value);
                    }
                    log_event(log_buffer);
                    
                    gen_mips_ast((yyvsp[0].node), 2);
                    sprintf(mips_buffer, "    sd r2, %s(r0)", var->name);
                    add_mips_instr(mips_buffer);
                }
            }
        }
        free((yyvsp[-2].str));
    }
#line 1266 "parser.tab.c"
    break;

  case 17: /* assignment: IDENTIFIER ASSIGN expression  */
#line 171 "parser.y"
                                 {
        char log_buffer[256];
        char mips_buffer[256];
        VARIABLE *var = find_variable((yyvsp[-2].str));
        
        if (var == NULL) {
            char buffer[200];
            sprintf(buffer, "Error: Variable '%s' is not declared!", (yyvsp[-2].str));
            add_error(buffer, line_number);
            sprintf(log_buffer, "[Line %d] ERROR: Failed to use '%s' (Not Declared)", line_number, (yyvsp[-2].str));
            log_event(log_buffer);
        } else if ((yyvsp[0].node) != NULL && !(yyvsp[0].node)->has_error) {
            int old_val = var->value;
            var->value = eval_ast((yyvsp[0].node));
            
            // Log with ASCII character if it's a char type and printable
            if (var->type == TYPE_CHAR && var->value >= 32 && var->value <= 126) {
                sprintf(log_buffer, "[Line %d] ASSIGN: Updated '%s' from %d to %d (ASCII: '%c')", 
                        line_number, var->name, old_val, var->value, (char)var->value);
            } else {
                sprintf(log_buffer, "[Line %d] ASSIGN: Updated '%s' from %d to %d", 
                        line_number, var->name, old_val, var->value);
            }
            log_event(log_buffer);
            
            gen_mips_ast((yyvsp[0].node), 2);
            sprintf(mips_buffer, "    sd r2, %s(r0)", var->name);
            add_mips_instr(mips_buffer);
        }
        free((yyvsp[-2].str));
    }
#line 1302 "parser.tab.c"
    break;

  case 18: /* expression_statement: expression  */
#line 205 "parser.y"
               {
        if ((yyvsp[0].node) != NULL && !(yyvsp[0].node)->has_error) {
            char log_buffer[256];
            int result = eval_ast((yyvsp[0].node));
            sprintf(log_buffer, "[Line %d] EXPRESSION: Evaluated standalone expression = %d", 
                    line_number, result);
            log_event(log_buffer);
            gen_mips_ast((yyvsp[0].node), 2);
        }
    }
#line 1317 "parser.tab.c"
    break;

  case 19: /* increment_decrement: IDENTIFIER INCREMENT  */
#line 218 "parser.y"
                         {
        char log_buffer[256];
        char mips_buffer[256];
        VARIABLE *var = find_variable((yyvsp[-1].str));
        
        if (var == NULL) {
            char buffer[200];
            sprintf(buffer, "Error: Variable '%s' is not declared!", (yyvsp[-1].str));
            add_error(buffer, line_number);
        } else {
            int old_val = var->value;
            var->value++;
            sprintf(log_buffer, "[Line %d] INCREMENT: Updated '%s' from %d to %d", 
                    line_number, var->name, old_val, var->value);
            log_event(log_buffer);
            
            sprintf(mips_buffer, "    ld r2, %s(r0)", var->name);
            add_mips_instr(mips_buffer);
            sprintf(mips_buffer, "    daddiu r2, r2, 1");
            add_mips_instr(mips_buffer);
            sprintf(mips_buffer, "    sd r2, %s(r0)", var->name);
            add_mips_instr(mips_buffer);
        }
        free((yyvsp[-1].str));
    }
#line 1347 "parser.tab.c"
    break;

  case 20: /* increment_decrement: IDENTIFIER DECREMENT  */
#line 243 "parser.y"
                           {
        char log_buffer[256];
        char mips_buffer[256];
        VARIABLE *var = find_variable((yyvsp[-1].str));
        
        if (var == NULL) {
            char buffer[200];
            sprintf(buffer, "Error: Variable '%s' is not declared!", (yyvsp[-1].str));
            add_error(buffer, line_number);
        } else {
            int old_val = var->value;
            var->value--;
            sprintf(log_buffer, "[Line %d] DECREMENT: Updated '%s' from %d to %d", 
                    line_number, var->name, old_val, var->value);
            log_event(log_buffer);
            
            sprintf(mips_buffer, "    ld r2, %s(r0)", var->name);
            add_mips_instr(mips_buffer);
            sprintf(mips_buffer, "    daddiu r2, r2, -1");
            add_mips_instr(mips_buffer);
            sprintf(mips_buffer, "    sd r2, %s(r0)", var->name);
            add_mips_instr(mips_buffer);
        }
        free((yyvsp[-1].str));
    }
#line 1377 "parser.tab.c"
    break;

  case 21: /* increment_decrement: INCREMENT IDENTIFIER  */
#line 268 "parser.y"
                           {
        char log_buffer[256];
        char mips_buffer[256];
        VARIABLE *var = find_variable((yyvsp[0].str));
        
        if (var == NULL) {
            char buffer[200];
            sprintf(buffer, "Error: Variable '%s' is not declared!", (yyvsp[0].str));
            add_error(buffer, line_number);
        } else {
            int old_val = var->value;
            var->value++;
            sprintf(log_buffer, "[Line %d] PRE-INCREMENT: Updated '%s' from %d to %d", 
                    line_number, var->name, old_val, var->value);
            log_event(log_buffer);
            
            sprintf(mips_buffer, "    ld r2, %s(r0)", var->name);
            add_mips_instr(mips_buffer);
            sprintf(mips_buffer, "    daddiu r2, r2, 1");
            add_mips_instr(mips_buffer);
            sprintf(mips_buffer, "    sd r2, %s(r0)", var->name);
            add_mips_instr(mips_buffer);
        }
        free((yyvsp[0].str));
    }
#line 1407 "parser.tab.c"
    break;

  case 22: /* increment_decrement: DECREMENT IDENTIFIER  */
#line 293 "parser.y"
                           {
        char log_buffer[256];
        char mips_buffer[256];
        VARIABLE *var = find_variable((yyvsp[0].str));
        
        if (var == NULL) {
            char buffer[200];
            sprintf(buffer, "Error: Variable '%s' is not declared!", (yyvsp[0].str));
            add_error(buffer, line_number);
        } else {
            int old_val = var->value;
            var->value--;
            sprintf(log_buffer, "[Line %d] PRE-DECREMENT: Updated '%s' from %d to %d", 
                    line_number, var->name, old_val, var->value);
            log_event(log_buffer);
            
            sprintf(mips_buffer, "    ld r2, %s(r0)", var->name);
            add_mips_instr(mips_buffer);
            sprintf(mips_buffer, "    daddiu r2, r2, -1");
            add_mips_instr(mips_buffer);
            sprintf(mips_buffer, "    sd r2, %s(r0)", var->name);
            add_mips_instr(mips_buffer);
        }
        free((yyvsp[0].str));
    }
#line 1437 "parser.tab.c"
    break;

  case 24: /* expression: expression PLUS term  */
#line 322 "parser.y"
                           {
        (yyval.node) = create_node(AST_OP, line_number);
        (yyval.node)->op = '+';
        (yyval.node)->left = (yyvsp[-2].node);
        (yyval.node)->right = (yyvsp[0].node);
        if (((yyvsp[-2].node) && (yyvsp[-2].node)->has_error) || ((yyvsp[0].node) && (yyvsp[0].node)->has_error)) {
            (yyval.node)->has_error = 1;
        }
    }
#line 1451 "parser.tab.c"
    break;

  case 25: /* expression: expression MINUS term  */
#line 331 "parser.y"
                            {
        (yyval.node) = create_node(AST_OP, line_number);
        (yyval.node)->op = '-';
        (yyval.node)->left = (yyvsp[-2].node);
        (yyval.node)->right = (yyvsp[0].node);
        if (((yyvsp[-2].node) && (yyvsp[-2].node)->has_error) || ((yyvsp[0].node) && (yyvsp[0].node)->has_error)) {
            (yyval.node)->has_error = 1;
        }
    }
#line 1465 "parser.tab.c"
    break;

  case 27: /* term: term MULTIPLY factor  */
#line 344 "parser.y"
                           {
        (yyval.node) = create_node(AST_OP, line_number);
        (yyval.node)->op = '*';
        (yyval.node)->left = (yyvsp[-2].node);
        (yyval.node)->right = (yyvsp[0].node);
        if (((yyvsp[-2].node) && (yyvsp[-2].node)->has_error) || ((yyvsp[0].node) && (yyvsp[0].node)->has_error)) {
            (yyval.node)->has_error = 1;
        }
    }
#line 1479 "parser.tab.c"
    break;

  case 28: /* term: term DIVIDE factor  */
#line 353 "parser.y"
                         {
        (yyval.node) = create_node(AST_OP, line_number);
        (yyval.node)->op = '/';
        (yyval.node)->left = (yyvsp[-2].node);
        (yyval.node)->right = (yyvsp[0].node);
        if (((yyvsp[-2].node) && (yyvsp[-2].node)->has_error) || ((yyvsp[0].node) && (yyvsp[0].node)->has_error)) {
            (yyval.node)->has_error = 1;
        }
    }
#line 1493 "parser.tab.c"
    break;

  case 29: /* factor: NUMBER  */
#line 365 "parser.y"
           {
        (yyval.node) = create_node(AST_NUM, line_number);
        (yyval.node)->int_val = (yyvsp[0].num);
    }
#line 1502 "parser.tab.c"
    break;

  case 30: /* factor: CHAR_LITERAL  */
#line 369 "parser.y"
                   {
        (yyval.node) = create_node(AST_NUM, line_number);
        (yyval.node)->int_val = (yyvsp[0].num);
    }
#line 1511 "parser.tab.c"
    break;

  case 31: /* factor: IDENTIFIER  */
#line 373 "parser.y"
                 {
        (yyval.node) = create_node(AST_VAR, line_number);
        (yyval.node)->var_name = strdup((yyvsp[0].str));
        
        VARIABLE *var = find_variable((yyvsp[0].str));
        if (var == NULL) {
            char buffer[200];
            sprintf(buffer, "Error: Variable '%s' is not declared!", (yyvsp[0].str));
            add_error(buffer, line_number);
            (yyval.node)->has_error = 1;
        }
        free((yyvsp[0].str));
    }
#line 1529 "parser.tab.c"
    break;

  case 32: /* factor: LPAREN expression RPAREN  */
#line 386 "parser.y"
                               {
        (yyval.node) = (yyvsp[-1].node);
    }
#line 1537 "parser.tab.c"
    break;

  case 33: /* factor: MINUS factor  */
#line 389 "parser.y"
                               {
        (yyval.node) = create_node(AST_OP, line_number);
        (yyval.node)->op = '-';
        ASTNode *zero = create_node(AST_NUM, line_number);
        zero->int_val = 0;
        (yyval.node)->left = zero;
        (yyval.node)->right = (yyvsp[0].node);
        if ((yyvsp[0].node) && (yyvsp[0].node)->has_error) {
            (yyval.node)->has_error = 1;
        }
    }
#line 1553 "parser.tab.c"
    break;

  case 34: /* factor: PLUS factor  */
#line 400 "parser.y"
                              {
        (yyval.node) = (yyvsp[0].node);
    }
#line 1561 "parser.tab.c"
    break;

  case 35: /* cdisplay_statement: CDISPLAY LPAREN expression RPAREN  */
#line 408 "parser.y"
                                      {
        if ((yyvsp[-1].node) != NULL && !(yyvsp[-1].node)->has_error) {
            char log_buffer[256];
            int result = eval_ast((yyvsp[-1].node));
            
            // Print to console
            printf("%d\n", result);
            
            // Log the event
            sprintf(log_buffer, "[Line %d] CDISPLAY: Printed value = %d", 
                    line_number, result);
            log_event(log_buffer);
            
            // Generate MIPS code to evaluate expression
            gen_mips_ast((yyvsp[-1].node), 2);
        }
    }
#line 1583 "parser.tab.c"
    break;

  case 36: /* cdisplay_statement: CDISPLAY LPAREN IDENTIFIER RPAREN  */
#line 425 "parser.y"
                                        {
        char log_buffer[256];
        VARIABLE *var = find_variable((yyvsp[-1].str));
        
        if (var == NULL) {
            char buffer[200];
            sprintf(buffer, "Error: Variable '%s' is not declared!", (yyvsp[-1].str));
            add_error(buffer, line_number);
        } else {
            // Print based on type
            if (var->type == TYPE_CHAR && var->value >= 32 && var->value <= 126) {
                printf("%c\n", (char)var->value);
                sprintf(log_buffer, "[Line %d] CDISPLAY: Printed '%s' = %d (ASCII: '%c')", 
                        line_number, var->name, var->value, (char)var->value);
            } else {
                printf("%d\n", var->value);
                sprintf(log_buffer, "[Line %d] CDISPLAY: Printed '%s' = %d", 
                        line_number, var->name, var->value);
            }
            log_event(log_buffer);
            
            // Generate MIPS code to load variable
            char mips_buffer[256];
            sprintf(mips_buffer, "    ld r2, %s(r0)", var->name);
            add_mips_instr(mips_buffer);
        }
        free((yyvsp[-1].str));
    }
#line 1616 "parser.tab.c"
    break;


#line 1620 "parser.tab.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

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
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
  ++yynerrs;

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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
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
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 455 "parser.y"




void yyerror(const char *s) {
    char buffer[256];
    sprintf(buffer, "Syntax error: %s", s);
    add_error(buffer, line_number);
}

// Helper function implementations
void log_event(const char *format, ...) {
    HISTORY *new_node = (HISTORY *)malloc(sizeof(HISTORY));
    new_node->msg = strdup(format);
    new_node->next = NULL;
    if (hist_head == NULL) { 
        hist_head = new_node; 
        hist_tail = new_node; 
    } else { 
        hist_tail->next = new_node; 
        hist_tail = new_node; 
    }
}

void add_mips_instr(char *code) {
    MIPS_INSTR *new_node = (MIPS_INSTR *)malloc(sizeof(MIPS_INSTR));
    new_node->code = strdup(code);
    new_node->next = NULL;
    if (mips_head == NULL) { 
        mips_head = new_node; 
        mips_tail = new_node; 
    } else { 
        mips_tail->next = new_node; 
        mips_tail = new_node; 
    }
}

VARIABLE *find_variable(char *name) {
    VARIABLE *current = var_head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) return current;
        current = current->next;
    }
    return NULL;
}

VARIABLE *add_variable(char *name, DataType type) {
    VARIABLE *new_var = (VARIABLE *)malloc(sizeof(VARIABLE));
    new_var->name = strdup(name);
    new_var->type = type;
    new_var->value = 0;
    new_var->next = NULL;
    if (var_head == NULL) { 
        var_head = new_var; 
        var_tail = new_var; 
    } else { 
        var_tail->next = new_var; 
        var_tail = new_var; 
    }
    return new_var;
}

void add_error(char *msg, int line) {
    ERROR *new_err = (ERROR *)malloc(sizeof(ERROR));
    new_err->msg = strdup(msg);
    new_err->line = line;
    new_err->next = NULL;
    if (err_head == NULL) { 
        err_head = new_err; 
        err_tail = new_err; 
    } else { 
        err_tail->next = new_err; 
        err_tail = new_err; 
    }
    has_errors = 1;
}

int is_valid_identifier(char *str) {
    if (str == NULL || str[0] == '\0') return 0;
    if (!isalpha(str[0]) && str[0] != '_') return 0;
    for (int i = 1; str[i] != '\0'; i++) {
        if (!isalnum(str[i]) && str[i] != '_') return 0;
    }
    return 1;
}

ASTNode* create_node(ASTNodeType type, int line) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    node->left = NULL;
    node->right = NULL;
    node->line = line;
    node->has_error = 0;
    node->int_val = 0;
    node->var_name = NULL;
    node->op = 0;
    return node;
}

int eval_ast(ASTNode *node) {
    if (node == NULL) return 0;
    if (node->has_error) return 0;
    
    if (node->type == AST_NUM) return node->int_val;
    if (node->type == AST_VAR) {
        VARIABLE *v = find_variable(node->var_name);
        if (v) return v->value;
        return 0;
    }
    if (node->type == AST_OP) {
        int left = eval_ast(node->left);
        int right = eval_ast(node->right);
        if (node->op == '+') return left + right;
        if (node->op == '-') return left - right;
        if (node->op == '*') return left * right;
        if (node->op == '/') return (right != 0) ? left / right : 0;
    }
    return 0;
}

void gen_mips_ast(ASTNode *node, int reg) {
    if (node == NULL || node->has_error) return;
    char buffer[100];

    if (node->type == AST_NUM) {
        sprintf(buffer, "    daddiu r%d, r0, %d", reg, node->int_val);
        add_mips_instr(buffer);
    } 
    else if (node->type == AST_VAR) {
        sprintf(buffer, "    ld r%d, %s(r0)", reg, node->var_name);
        add_mips_instr(buffer);
    } 
    else if (node->type == AST_OP) {
        gen_mips_ast(node->left, reg);
        gen_mips_ast(node->right, reg + 1);

        if (node->op == '+') {
            sprintf(buffer, "    daddu r%d, r%d, r%d", reg, reg, reg + 1);
        }
        else if (node->op == '-') {
            sprintf(buffer, "    dsubu r%d, r%d, r%d", reg, reg, reg + 1);
        }
        else if (node->op == '*') {
            sprintf(buffer, "    dmult r%d, r%d", reg, reg + 1);
            add_mips_instr(buffer);
            sprintf(buffer, "    mflo r%d", reg);
        }
        else if (node->op == '/') {
            sprintf(buffer, "    ddiv r%d, r%d", reg, reg + 1);
            add_mips_instr(buffer);
            sprintf(buffer, "    mflo r%d", reg);
        }
        add_mips_instr(buffer);
    }
}

void generate_mips_file(const char* filename) {
    if (has_errors) {
        printf(">> MIPS64 code generation skipped due to errors.\n");
        return;
    }
    
    FILE *f = fopen(filename, "w");
    if (f == NULL) { 
        printf("Error writing MIPS file.\n"); 
        return; 
    }

    fprintf(f, "    .data\n");
    VARIABLE *v = var_head;
    while (v != NULL) {
        fprintf(f, "%s: .word 0, 0\n", v->name);
        v = v->next;
    }

    fprintf(f, "\n    .text\n");
    fprintf(f, "main:\n");

    MIPS_INSTR *m = mips_head;
    while (m != NULL) {
        fprintf(f, "%s\n", m->code);
        m = m->next;
    }
    
    fclose(f);
    printf(">> MIPS64 code generated in '%s'\n", filename);
}

void print_results() {
    printf("\n=== CHRONOLOGICAL HISTORY ===\n");
    HISTORY *h = hist_head;
    while (h != NULL) { 
        printf("  %s\n", h->msg); 
        h = h->next; 
    }

    printf("\n=== SYMBOL TABLE ===\n");
    VARIABLE *v = var_head;
    if (v == NULL) printf("  (No variables declared)\n");
    while (v != NULL) {
        char *type_str = (v->type == TYPE_INT) ? "INT" : "CHAR";
        if (v->type == TYPE_CHAR && v->value >= 32 && v->value <= 126) {
            printf("  VAR: %-10s | TYPE: %-4s | VAL: %-3d (ASCII: '%c')\n", 
                   v->name, type_str, v->value, (char)v->value);
        } else {
            printf("  VAR: %-10s | TYPE: %-4s | VAL: %d\n", 
                   v->name, type_str, v->value);
        }
        v = v->next;
    }

    printf("\n=== ERROR LOG ===\n");
    ERROR *e = err_head;
    if (e == NULL) printf("  (No errors found)\n");
    while (e != NULL) { 
        printf("  Line %d: %s\n", e->line, e->msg); 
        e = e->next; 
    }
    
    generate_mips_file("output.txt");
}

void cleanup_all() {
    VARIABLE *v = var_head;
    while (v != NULL) {
        VARIABLE *next = v->next;
        free(v->name);
        free(v);
        v = next;
    }
    var_head = NULL;
    var_tail = NULL;
    
    ERROR *e = err_head;
    while (e != NULL) {
        ERROR *next = e->next;
        free(e->msg);
        free(e);
        e = next;
    }
    err_head = NULL;
    err_tail = NULL;
    
    HISTORY *h = hist_head;
    while (h != NULL) {
        HISTORY *next = h->next;
        free(h->msg);
        free(h);
        h = next;
    }
    hist_head = NULL;
    hist_tail = NULL;
    
    MIPS_INSTR *m = mips_head;
    while (m != NULL) {
        MIPS_INSTR *next = m->next;
        free(m->code);
        free(m);
        m = next;
    }
    mips_head = NULL;
    mips_tail = NULL;
    
    line_number = 1;
    has_errors = 0;
}

void read_output_file(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open file '%s'\n", filename);
        return;
    }
    char line[256];
    printf("\n\n=== CONTENTS OF '%s' ===\n", filename);
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
    printf("=== END OF FILE ===\n\n");
}

// --- FUNCTION TO CREATE EMPTY FILE ---
void create_empty_file(const char* filename) {
    FILE *file = fopen(filename, "w");
    if (file != NULL) {
        fclose(file);
    }
}

void show_errors(){
    if(err_head == NULL){
        return;
    }

    printf("\n=== ERROR LOG ===\n"); // Added header for readability
    ERROR *current_err = err_head;
    
    // BUG FIX: Changed from (!current_err) to (current_err != NULL)
    while(current_err != NULL){ 
        printf("[LINE %d]: %s\n", current_err->line, current_err->msg);
        current_err = current_err->next;
    }
}

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            perror(argv[1]);
            return 1;
        }
        yyin = file;
    }

    // 1. Parse the file
    yyparse();

    // 2. Check for errors immediately after parsing
    if(has_errors || err_head != NULL) {
        // Show the errors found during parsing/lexing
        show_errors();

        printf("\n>> Errors detected. Creating empty output files...\n");
        
        // Clear files because build failed
        create_empty_file("output.txt");
        create_empty_file("program.txt");
        
        printf(">> All output files cleared due to errors.\n");
        printf(">> Please fix the errors above and try again.\n");
    } 
    else {
        // 3. No errors: Proceed with Code Generation
        
        // Generate Assembly Text
        generate_mips_file("output.txt");
        
        // Generate Binary (assuming this function is defined in mips_bin.h)
        // Ensure this function exists in your included headers
        generate_binary_file("output.txt", "program.txt"); 
        
        printf(">> Program finished successfully.\n");
    }

    // 4. Cleanup memory
    cleanup_all();
    
    return 0;
}
