/* original parser id follows */
/* yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93" */
/* (use YYMAJOR/YYMINOR for ifdefs dependent on parser version) */

#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)
#define YYENOMEM       (-2)
#define YYEOF          0
#undef YYBTYACC
#define YYBTYACC 0
#define YYDEBUGSTR YYPREFIX "debug"
#define YYPREFIX "yy"

#define YYPURE 0

#line 2 "testlang_parse.y"
/*	$NetBSD: testlang_parse.y,v 1.17 2020/04/23 00:30:08 joerg Exp $	*/

/*-
 * Copyright 2009 Brett Lymn <blymn@NetBSD.org>
 *
 * All rights reserved.
 *
 * This code has been donated to The NetBSD Foundation by the Author.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. The name of the author may not be used to endorse or promote products
 *    derived from this software withough specific prior written permission
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 */
#include <assert.h>
#include <curses.h>
#include <errno.h>
#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <poll.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <vis.h>
#include <stdint.h>
#include "returns.h"
#include "flags.h"

#define YYDEBUG 1

extern int verbose;
extern int check_file_flag;
extern int cmdpipe[2];
extern int slvpipe[2];
extern int master;
extern struct pollfd readfd;
extern char *check_path;
extern char *cur_file;		/* from director.c */

int yylex(void);

size_t line;

static int input_delay;

/* time delay between inputs chars - default to 0.1ms minimum to prevent
 * problems with input tests
 */
#define DELAY_MIN 0.1

/* time delay after a function call - allows the slave time to
 * run the function and output data before we do other actions.
 * Set this to 50ms.
 */
#define POST_CALL_DELAY 50

static struct timespec delay_spec = {0, 1000 * DELAY_MIN};
static struct timespec delay_post_call = {0, 1000 * POST_CALL_DELAY};

static char *input_str;	/* string to feed in as input */
static bool no_input;	/* don't need more input */

static wchar_t *vals = NULL;	/* wchars to attach to a cchar type */
static unsigned nvals;		/* number of wchars */

#define READ_PIPE  0
#define WRITE_PIPE 1

const char *enum_names[] = {
	"unused", "static", "numeric", "string", "byte", "cchar", "ERR", "OK", "NULL",
	"not NULL", "variable", "reference", "returns count", "slave error"
};

typedef struct {
	data_enum_t	arg_type;
	size_t		arg_len;
	char		*arg_string;
	int		var_index;
} args_t;

typedef struct {
	char		*function;
	int		nrets;		/* number of returns */
	ct_data_t	*returns;	/* array of expected returns */
	int		nargs;		/* number of arguments */
	args_t		*args;		/* arguments for the call */
} cmd_line_t;

static cmd_line_t	command;

typedef struct {
	char *name;
	size_t len;
	data_enum_t type;
	void *value;
	cchar_t cchar;
} var_t;

static size_t nvars; 		/* Number of declared variables */
static var_t *vars; 		/* Variables defined during the test. */

static int	check_function_table(char *, const char *[], int);
static int	find_var_index(const char *);
static void 	assign_arg(data_enum_t, void *);
static int	assign_var(char *);
void		init_parse_variables(int);
static void	validate(int, void *);
static void	validate_return(const char *, const char *, int);
static void	validate_variable(int, data_enum_t, const void *, int, int);
static void	validate_byte(ct_data_t *, ct_data_t *, int);
static void	validate_cchar(var_t *, var_t *, int);
static void	write_cmd_pipe(char *);
static void	write_cmd_pipe_args(data_enum_t, void *);
static void	read_cmd_pipe(ct_data_t *);
static void	write_func_and_args(void);
static void	compare_streams(char *, bool);
static void	do_function_call(size_t);
static void	save_slave_output(bool);
static void	validate_type(data_enum_t, ct_data_t *, int);
static void	set_var(data_enum_t, char *, void *);
static void	validate_reference(int, void *);
static char	*numeric_or(char *, char *);
static char	*get_numeric_var(const char *);
static void	perform_delay(struct timespec *);
static void	set_cchar(char *, void *);
static wchar_t	*add_to_vals(data_enum_t, void *);

static const char *input_functions[] = {
	"getch", "getnstr", "getstr", "mvgetnstr", "mvgetstr", "mvgetnstr",
	"mvgetstr", "mvscanw", "mvwscanw", "scanw", "wgetch", "wgetnstr",
	"wgetstr"
};

static const unsigned ninput_functions =
	sizeof(input_functions) / sizeof(char *);

extern saved_data_t saved_output;

#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#line 162 "testlang_parse.y"
typedef union {
	char *string;
	ct_data_t *retval;
	wchar_t	*vals;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
#line 195 "testlang_parse.c"

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() yylex(void *YYLEX_PARAM)
# define YYLEX yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(void)
# define YYLEX yylex()
#endif

#if !(defined(yylex) || defined(YYSTATE))
int YYLEX_DECL();
#endif

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(msg)
#endif

extern int YYPARSE_DECL();

#define PATH 257
#define STRING 258
#define BYTE 259
#define VARNAME 260
#define FILENAME 261
#define VARIABLE 262
#define REFERENCE 263
#define NULL_RET 264
#define NON_NULL 265
#define ERR_RET 266
#define OK_RET 267
#define numeric 268
#define DELAY 269
#define INPUT 270
#define COMPARE 271
#define COMPAREND 272
#define ASSIGN 273
#define CCHAR 274
#define EOL 275
#define CALL 276
#define CHECK 277
#define NOINPUT 278
#define OR 279
#define MULTIPLIER 280
#define LHB 281
#define RHB 282
#define LHSB 283
#define RHSB 284
#define CALL2 285
#define CALL3 286
#define CALL4 287
#define DRAIN 288
#define YYERRCODE 256
typedef int YYINT;
static const YYINT yylhs[] = {                           -1,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   14,    1,   16,    1,   17,    1,
   18,    1,   12,   19,   19,   19,   20,   20,   20,   20,
   20,    2,    3,    4,    5,    6,    7,    8,    9,   10,
   10,   11,   11,   22,   22,   22,   26,   26,   26,   26,
   26,   26,   26,   26,   26,   25,   27,   23,   21,   21,
   21,   21,   21,   21,   21,   21,   15,   15,   15,   24,
   28,   24,   29,   24,   30,   24,   31,   24,   32,   24,
   33,   24,   34,   24,   35,   24,   36,   24,   13,
};
static const YYINT yylen[] = {                            2,
    0,    2,    2,    2,    2,    2,    2,    2,    2,    2,
    2,    2,    2,    2,    0,    5,    0,    7,    0,    5,
    0,    5,    5,    1,    3,    1,    1,    3,    1,    1,
    1,    5,    6,    7,    8,    4,    3,    3,    2,    3,
    3,    3,    3,    1,    1,    1,    1,    3,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    3,    3,    3,    3,    2,    1,    1,    3,    0,
    0,    5,    0,    3,    0,    3,    0,    3,    0,    3,
    0,    3,    0,    3,    0,    3,    0,    3,    1,
};
static const YYINT yydefred[] = {                         0,
    0,    0,    0,    0,    0,    0,   89,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   49,   50,   56,   57,   53,
   54,   51,   52,   47,    0,    0,   45,   44,   46,    0,
   39,    0,    0,    0,    2,    3,    4,    5,    6,    7,
    8,    9,   10,   11,   12,   13,   14,   37,   38,   40,
   41,   42,   43,   19,   21,   15,    0,   26,   24,    0,
    0,   68,   67,    0,   58,    0,   55,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   30,   31,   29,   27,
    0,    0,    0,   48,   79,   75,   77,   83,   81,   85,
   87,   73,    0,    0,   36,    0,    0,    0,   20,   22,
   16,   17,   25,    0,    0,    0,    0,    0,   23,   69,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   32,
    0,    0,    0,    0,    0,    0,    0,    0,   28,    0,
   80,   76,   78,   84,   82,   86,   88,   74,   71,   33,
    0,    0,   18,   65,   64,   63,   62,    0,   34,    0,
   72,   35,
};
#if defined(YYDESTRUCT_CALL) || defined(YYSTYPE_TOSTRING)
static const YYINT yystos[] = {                           0,
  269,  270,  271,  272,  273,  274,  275,  276,  277,  278,
  285,  286,  287,  290,  291,  292,  293,  294,  295,  296,
  297,  298,  299,  300,  301,  302,  303,  268,  258,  257,
  261,  257,  261,  260,  260,  258,  259,  260,  262,  264,
  265,  266,  267,  268,  281,  312,  315,  316,  317,  315,
  303,  312,  312,  312,  290,  290,  290,  290,  290,  290,
  290,  290,  290,  290,  290,  290,  290,  303,  303,  303,
  303,  303,  303,  258,  259,  268,  281,  262,  268,  281,
  309,  262,  268,  305,  260,  313,  315,  316,  312,  312,
  312,  307,  308,  304,  305,  305,  258,  259,  262,  268,
  283,  310,  279,  282,  257,  258,  259,  260,  261,  262,
  264,  268,  281,  314,  303,  313,  312,  312,  303,  303,
  303,  282,  282,  258,  259,  262,  268,  311,  303,  305,
  322,  320,  321,  324,  323,  325,  326,  319,  305,  303,
  314,  313,  312,  306,  280,  280,  280,  280,  284,  311,
  314,  314,  314,  314,  314,  314,  314,  314,  282,  303,
  314,  313,  303,  268,  268,  268,  268,  318,  303,  314,
  314,  303,
};
#endif /* YYDESTRUCT_CALL || YYSTYPE_TOSTRING */
static const YYINT yydgoto[] = {                         14,
   15,   16,   17,   18,   19,   20,   21,   22,   23,   24,
   25,   26,   27,   94,   84,  144,   92,   93,   81,  102,
  150,   46,   86,  114,   47,   48,   49,  168,  138,  132,
  133,  131,  135,  134,  136,  137,
};
static const YYINT yysindex[] = {                      -108,
 -252, -231, -233, -215, -235, -208,    0, -122, -204, -199,
 -122, -122, -122,    0, -108, -108, -108, -108, -108, -108,
 -108, -108, -108, -108, -108, -108, -108, -199, -199, -199,
 -199, -199, -199, -170, -223,    0,    0,    0,    0,    0,
    0,    0,    0,    0, -224, -200,    0,    0,    0, -255,
    0, -122, -122, -122,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, -224,    0,    0, -224,
 -156,    0,    0, -220,    0, -142,    0, -199, -200, -122,
 -122, -199, -199, -199, -213, -209,    0,    0,    0,    0,
 -110, -199, -224,    0,    0,    0,    0,    0,    0,    0,
    0,    0, -224, -199,    0, -142, -200, -122,    0,    0,
    0,    0,    0, -229, -193, -189, -186, -205,    0,    0,
 -142, -142, -142, -142, -142, -142, -142, -142, -202,    0,
 -199, -142, -200, -199, -175, -168, -167, -161,    0, -110,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 -199, -142,    0,    0,    0,    0,    0, -142,    0, -199,
    0,    0,
};
static const YYINT yyrindex[] = {                       108,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  108,  108,  108,  108,  108,  108,
  108,  108,  108,  108,  108,  108,  108,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, -166,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, -166,    0,    0,    0,    0,
    0,    0,    0,    0, -194, -187, -163,    0,    0,    0,
 -166, -166, -166, -166, -166, -166, -166, -166,    0,    0,
    0, -166,    0,    0,    0,    0,    0,    0,    0, -174,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, -166,    0,    0,    0,    0,    0, -166,    0,    0,
    0,    0,
};
#if YYBTYACC
static const YYINT yycindex[] = {                         0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,
};
#endif
static const YYINT yygindex[] = {                       165,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  -10,    0,  -63,    0,    0,    0,    0,    0,
   13,   -5,  -88, -101,   -7,   73,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,
};
#define YYTABLESIZE 192
static const YYINT yytable[] = {                         51,
  116,   50,   36,   37,   38,   52,   53,   54,   40,   41,
   42,   43,   44,   95,  141,   28,   96,   68,   69,   70,
   71,   72,   73,   30,   34,   45,   29,   31,  142,  151,
  152,  153,  154,  155,  156,  157,  158,   82,   78,  130,
  161,   32,   87,   83,   79,   33,   89,   90,   91,  139,
  145,   35,  124,  125,  162,   38,  126,   80,  103,   85,
  170,  104,  127,   61,   61,  103,  171,   61,  122,  103,
   60,   60,  123,   61,   60,    7,  103,  115,  149,  159,
   60,  119,  120,  121,  117,  118,  146,   74,   75,   61,
  147,  129,  164,  148,   59,   59,   60,   76,   59,  165,
  166,   97,   98,  140,   59,   99,  167,    1,   70,   66,
   77,  100,  143,  128,  105,  106,  107,  108,  109,  110,
   59,  111,   88,    0,    0,  112,  101,    0,    0,    0,
  160,    0,    0,  163,    0,   36,   37,   38,  113,   39,
    0,   40,   41,   42,   43,   44,    0,  124,  125,    0,
  169,  126,    0,    0,    0,    0,    0,  127,   45,  172,
    1,    2,    3,    4,    5,    6,    7,    8,    9,   10,
    0,    0,    0,    0,    0,    0,   11,   12,   13,   55,
   56,   57,   58,   59,   60,   61,   62,   63,   64,   65,
   66,   67,
};
static const YYINT yycheck[] = {                         10,
   89,    9,  258,  259,  260,   11,   12,   13,  264,  265,
  266,  267,  268,   77,  116,  268,   80,   28,   29,   30,
   31,   32,   33,  257,  260,  281,  258,  261,  117,  131,
  132,  133,  134,  135,  136,  137,  138,  262,  262,  103,
  142,  257,   50,  268,  268,  261,   52,   53,   54,  113,
  280,  260,  258,  259,  143,  260,  262,  281,  279,  260,
  162,  282,  268,  258,  259,  279,  168,  262,  282,  279,
  258,  259,  282,  268,  262,  275,  279,   88,  284,  282,
  268,   92,   93,   94,   90,   91,  280,  258,  259,  284,
  280,  102,  268,  280,  258,  259,  284,  268,  262,  268,
  268,  258,  259,  114,  268,  262,  268,    0,  275,  284,
  281,  268,  118,  101,  257,  258,  259,  260,  261,  262,
  284,  264,   50,   -1,   -1,  268,  283,   -1,   -1,   -1,
  141,   -1,   -1,  144,   -1,  258,  259,  260,  281,  262,
   -1,  264,  265,  266,  267,  268,   -1,  258,  259,   -1,
  161,  262,   -1,   -1,   -1,   -1,   -1,  268,  281,  170,
  269,  270,  271,  272,  273,  274,  275,  276,  277,  278,
   -1,   -1,   -1,   -1,   -1,   -1,  285,  286,  287,   15,
   16,   17,   18,   19,   20,   21,   22,   23,   24,   25,
   26,   27,
};
#if YYBTYACC
static const YYINT yyctable[] = {                        -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,
};
#endif
#define YYFINAL 14
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 288
#define YYUNDFTOKEN 327
#define YYTRANSLATE(a) ((a) > YYMAXTOKEN ? YYUNDFTOKEN : (a))
#if YYDEBUG
static const char *const yyname[] = {

"$end",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"error","PATH","STRING","BYTE","VARNAME",
"FILENAME","VARIABLE","REFERENCE","NULL_RET","NON_NULL","ERR_RET","OK_RET",
"numeric","DELAY","INPUT","COMPARE","COMPAREND","ASSIGN","CCHAR","EOL","CALL",
"CHECK","NOINPUT","OR","MULTIPLIER","LHB","RHB","LHSB","RHSB","CALL2","CALL3",
"CALL4","DRAIN","$accept","statement","assign","call","call2","call3","call4",
"check","delay","input","noinput","compare","comparend","cchar","eol","$$1",
"expr","$$2","$$3","$$4","attributes","char_vals","array","result","fn_name",
"args","var","returns","reference","$$5","$$6","$$7","$$8","$$9","$$10","$$11",
"$$12","$$13","illegal-symbol",
};
static const char *const yyrule[] = {
"$accept : statement",
"statement :",
"statement : assign statement",
"statement : call statement",
"statement : call2 statement",
"statement : call3 statement",
"statement : call4 statement",
"statement : check statement",
"statement : delay statement",
"statement : input statement",
"statement : noinput statement",
"statement : compare statement",
"statement : comparend statement",
"statement : cchar statement",
"statement : eol statement",
"$$1 :",
"assign : ASSIGN VARNAME numeric $$1 eol",
"$$2 :",
"assign : ASSIGN VARNAME LHB expr RHB $$2 eol",
"$$3 :",
"assign : ASSIGN VARNAME STRING $$3 eol",
"$$4 :",
"assign : ASSIGN VARNAME BYTE $$4 eol",
"cchar : CCHAR VARNAME attributes char_vals eol",
"attributes : numeric",
"attributes : LHB expr RHB",
"attributes : VARIABLE",
"char_vals : numeric",
"char_vals : LHSB array RHSB",
"char_vals : VARIABLE",
"char_vals : STRING",
"char_vals : BYTE",
"call : CALL result fn_name args eol",
"call2 : CALL2 result result fn_name args eol",
"call3 : CALL3 result result result fn_name args eol",
"call4 : CALL4 result result result result fn_name args eol",
"check : CHECK var returns eol",
"delay : DELAY numeric eol",
"input : INPUT STRING eol",
"noinput : NOINPUT eol",
"compare : COMPARE PATH eol",
"compare : COMPARE FILENAME eol",
"comparend : COMPAREND PATH eol",
"comparend : COMPAREND FILENAME eol",
"result : returns",
"result : var",
"result : reference",
"returns : numeric",
"returns : LHB expr RHB",
"returns : STRING",
"returns : BYTE",
"returns : ERR_RET",
"returns : OK_RET",
"returns : NULL_RET",
"returns : NON_NULL",
"returns : var",
"var : VARNAME",
"reference : VARIABLE",
"fn_name : VARNAME",
"array : numeric",
"array : VARIABLE",
"array : BYTE",
"array : numeric MULTIPLIER numeric",
"array : VARIABLE MULTIPLIER numeric",
"array : BYTE MULTIPLIER numeric",
"array : STRING MULTIPLIER numeric",
"array : array array",
"expr : numeric",
"expr : VARIABLE",
"expr : expr OR expr",
"args :",
"$$5 :",
"args : LHB expr RHB $$5 args",
"$$6 :",
"args : numeric $$6 args",
"$$7 :",
"args : STRING $$7 args",
"$$8 :",
"args : BYTE $$8 args",
"$$9 :",
"args : PATH $$9 args",
"$$10 :",
"args : FILENAME $$10 args",
"$$11 :",
"args : VARNAME $$11 args",
"$$12 :",
"args : VARIABLE $$12 args",
"$$13 :",
"args : NULL_RET $$13 args",
"eol : EOL",

};
#endif

#if YYDEBUG
int      yydebug;
#endif

int      yyerrflag;
int      yychar;
YYSTYPE  yyval;
YYSTYPE  yylval;
int      yynerrs;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
YYLTYPE  yyloc; /* position returned by actions */
YYLTYPE  yylloc; /* position from the lexer */
#endif

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
#ifndef YYLLOC_DEFAULT
#define YYLLOC_DEFAULT(loc, rhs, n) \
do \
{ \
    if (n == 0) \
    { \
        (loc).first_line   = YYRHSLOC(rhs, 0).last_line; \
        (loc).first_column = YYRHSLOC(rhs, 0).last_column; \
        (loc).last_line    = YYRHSLOC(rhs, 0).last_line; \
        (loc).last_column  = YYRHSLOC(rhs, 0).last_column; \
    } \
    else \
    { \
        (loc).first_line   = YYRHSLOC(rhs, 1).first_line; \
        (loc).first_column = YYRHSLOC(rhs, 1).first_column; \
        (loc).last_line    = YYRHSLOC(rhs, n).last_line; \
        (loc).last_column  = YYRHSLOC(rhs, n).last_column; \
    } \
} while (0)
#endif /* YYLLOC_DEFAULT */
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#if YYBTYACC

#ifndef YYLVQUEUEGROWTH
#define YYLVQUEUEGROWTH 32
#endif
#endif /* YYBTYACC */

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH  10000
#endif
#endif

#ifndef YYINITSTACKSIZE
#define YYINITSTACKSIZE 200
#endif

typedef struct {
    unsigned stacksize;
    YYINT    *s_base;
    YYINT    *s_mark;
    YYINT    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE  *p_base;
    YYLTYPE  *p_mark;
#endif
} YYSTACKDATA;
#if YYBTYACC

struct YYParseState_s
{
    struct YYParseState_s *save;    /* Previously saved parser state */
    YYSTACKDATA            yystack; /* saved parser stack */
    int                    state;   /* saved parser state */
    int                    errflag; /* saved error recovery status */
    int                    lexeme;  /* saved index of the conflict lexeme in the lexical queue */
    YYINT                  ctry;    /* saved index in yyctable[] for this conflict */
};
typedef struct YYParseState_s YYParseState;
#endif /* YYBTYACC */
/* variables for the parser stack */
static YYSTACKDATA yystack;
#if YYBTYACC

/* Current parser state */
static YYParseState *yyps = 0;

/* yypath != NULL: do the full parse, starting at *yypath parser state. */
static YYParseState *yypath = 0;

/* Base of the lexical value queue */
static YYSTYPE *yylvals = 0;

/* Current position at lexical value queue */
static YYSTYPE *yylvp = 0;

/* End position of lexical value queue */
static YYSTYPE *yylve = 0;

/* The last allocated position at the lexical value queue */
static YYSTYPE *yylvlim = 0;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
/* Base of the lexical position queue */
static YYLTYPE *yylpsns = 0;

/* Current position at lexical position queue */
static YYLTYPE *yylpp = 0;

/* End position of lexical position queue */
static YYLTYPE *yylpe = 0;

/* The last allocated position at the lexical position queue */
static YYLTYPE *yylplim = 0;
#endif

/* Current position at lexical token queue */
static YYINT  *yylexp = 0;

static YYINT  *yylexemes = 0;
#endif /* YYBTYACC */
#line 532 "testlang_parse.y"

static void
excess(const char *fname, size_t lineno, const char *func, const char *comment,
    const void *data, size_t datalen)
{
	size_t dstlen = datalen * 4 + 1;
	char *dst = malloc(dstlen);

	if (dst == NULL)
		err(1, "malloc");

	if (strnvisx(dst, dstlen, data, datalen, VIS_WHITE | VIS_OCTAL) == -1)
		err(1, "strnvisx");

	warnx("%s, %zu: [%s] Excess %zu bytes%s [%s]",
	    fname, lineno, func, datalen, comment, dst);
	free(dst);
}

/*
 * Get the value of a variable, error if the variable has not been set or
 * is not a numeric type.
 */
static char *
get_numeric_var(const char *var)
{
	int i;

	if ((i = find_var_index(var)) < 0)
		err(1, "Variable %s is undefined", var);

	if (vars[i].type != data_number)
		err(1, "Variable %s is not a numeric type", var);

	return vars[i].value;
}

/*
 * Perform a bitwise OR on two numbers and return the result.
 */
static char *
numeric_or(char *n1, char *n2)
{
	unsigned long i1, i2, result;
	char *ret;

	i1 = strtoul(n1, NULL, 10);
	i2 = strtoul(n2, NULL, 10);

	result = i1 | i2;
	asprintf(&ret, "%lu", result);

	if (verbose) {
		fprintf(stderr, "numeric or of 0x%lx (%s) and 0x%lx (%s)"
		    " results in 0x%lx (%s)\n",
		    i1, n1, i2, n2, result, ret);
	}

	return ret;
}

/*
 * Sleep for the specified time, handle the sleep getting interrupted
 * by a signal.
 */
static void
perform_delay(struct timespec *ts)
{
	struct timespec delay_copy, delay_remainder;

	delay_copy = *ts;
	while (nanosleep(&delay_copy, &delay_remainder) < 0) {
		if (errno != EINTR)
			err(2, "nanosleep returned error");
		delay_copy = delay_remainder;
	}
}

/*
 * Add to temporary vals array
 */
static wchar_t	*add_to_vals(data_enum_t argtype, void *arg)
{
	wchar_t *retval = NULL;
	int have_malloced;
	int i;
	ct_data_t *ret;

	have_malloced = 0;

	if (nvals == 0) {
		have_malloced = 1;
		retval = malloc(sizeof(wchar_t));
	} else {
		retval = realloc(vals, (nvals + 1) * sizeof(wchar_t));
	}

	if (retval == NULL)
		return retval;

	vals = retval;

	switch (argtype) {
	case data_number:
		vals[nvals++] = (wchar_t) strtoul((char *) arg, NULL, 10);
		break;

	case data_string:
		vals[nvals++] = (wchar_t) ((char *)arg)[0];
		break;

	case data_byte:
		ret = (ct_data_t *) arg;
		vals[nvals++] = *((wchar_t *) ret->data_value);
		break;

	case data_var:
		if ((i = find_var_index((char *) arg)) < 0)
			err(1, "Variable %s is undefined at line %zu "
			    "of file %s", (char *) arg, line, cur_file);

		switch (vars[i].type) {

		case data_number:
		case data_string:
		case data_byte:
			retval = add_to_vals(vars[i].type, vars[i].value);
			break;

		default:
			err(1, "Variable %s is not a valid type for cchar"
			    " at line %zu of file %s", (char *) arg, line,
			    cur_file);
			break;

		}
		break;

	default:
		err(1, "add_to_vals: Unhandled type for vals array "
		    "at line %zu of file %s", line, cur_file);

		/* if we get here without a value then tidy up */
		if ((nvals == 0) && (have_malloced == 1)) {
			free(retval);
			retval = vals;
		}
		break;

	}

	return retval;
}

/*
 * Assign the value given to the named variable.
 */
static void
set_var(data_enum_t type, char *name, void *value)
{
	int i;
	char *number;
	ct_data_t *ret;

	i = find_var_index(name);
	if (i < 0)
		i = assign_var(name);

	vars[i].type = type;
	if ((type == data_number) || (type == data_string)) {
		number = value;
		vars[i].len = strlen(number) + 1;
		vars[i].value = malloc(vars[i].len + 1);
		if (vars[i].value == NULL)
			err(1, "Could not malloc memory for assign string");
		strcpy(vars[i].value, number);
	} else {
		/* can only be a byte value */
		ret = value;
		vars[i].len = ret->data_len;
		vars[i].value = malloc(vars[i].len);
		if (vars[i].value == NULL)
			err(1, "Could not malloc memory to assign byte string");
		memcpy(vars[i].value, ret->data_value, vars[i].len);
	}
}

/*
 * Form up a complex character type from the given components.
 */
static void
set_cchar(char *name, void *attributes)
{
	int i;
	unsigned j;
	attr_t attribs;

	if (nvals >= CURSES_CCHAR_MAX)
		err(1, "%s: too many characters in complex char type at "
			"line %zu of file %s", __func__, line, cur_file);

	i = find_var_index(name);
	if (i < 0)
		i = assign_var(name);

	if (sscanf((char *) attributes, "%d", &attribs) != 1)
		err(1, "%s: conversion of attributes to integer failed at"
			"line %zu of file %s", __func__, line, cur_file);

	vars[i].type = data_cchar;
	vars[i].cchar.attributes = attribs;
	vars[i].cchar.elements = nvals;
	for (j = 0; j < nvals; j++) 
		vars[i].cchar.vals[j] = vals[j];

	nvals = 0;
	vals = NULL;

}

/*
 * Add a new variable to the vars array, the value will be assigned later,
 * when a test function call returns.
 */
static int
assign_var(char *varname)
{
	var_t *temp;
	char *name;

	if ((name = malloc(strlen(varname) + 1)) == NULL)
		err(1, "Alloc of varname failed");

	if ((temp = realloc(vars, sizeof(*temp) * (nvars + 1))) == NULL) {
		free(name);
		err(1, "Realloc of vars array failed");
	}

	strcpy(name, varname);
	vars = temp;
	vars[nvars].name = name;
	vars[nvars].len = 0;
	vars[nvars].value = NULL;
	nvars++;

	return (nvars - 1);
}

/*
 * Allocate and assign a new argument of the given type.
 */
static void
assign_arg(data_enum_t arg_type, void *arg)
{
	args_t *temp, cur;
	char *str = arg;
	ct_data_t *ret;

	if (verbose) {
		fprintf(stderr, "function is >%s<, adding arg >%s< type %s (%d)\n",
		       command.function, str, enum_names[arg_type], arg_type);
	}

	cur.arg_type = arg_type;
	if (cur.arg_type == data_var) {
		cur.var_index = find_var_index(arg);
		if (cur.var_index < 0)
			err(1, "Invalid variable %s at line %zu of file %s",
			    str, line, cur_file);
	} else if (cur.arg_type == data_byte) {
		ret = arg;
		cur.arg_len = ret->data_len;
		cur.arg_string = malloc(cur.arg_len);
		if (cur.arg_string == NULL)
			err(1, "Could not malloc memory for arg bytes");
		memcpy(cur.arg_string, ret->data_value, cur.arg_len);
	} else if (cur.arg_type == data_null) {
		cur.arg_len = 0;
		cur.arg_string = NULL;
	} else {
		cur.arg_len = strlen(str);
		cur.arg_string = malloc(cur.arg_len + 1);
		if (cur.arg_string == NULL)
			err(1, "Could not malloc memory for arg string");
		strcpy(cur.arg_string, arg);
	}

	temp = realloc(command.args, sizeof(*temp) * (command.nargs + 1));
	if (temp == NULL)
		err(1, "Failed to reallocate args");
	command.args = temp;
	memcpy(&command.args[command.nargs], &cur, sizeof(args_t));
	command.nargs++;
}

/*
 * Allocate and assign a new return.
 */
static void
assign_rets(data_enum_t ret_type, void *ret)
{
	ct_data_t *temp, cur;
	char *ret_str;
	ct_data_t *ret_ret;

	if(ret_type == data_var)
		fprintf(stderr, " ----- %s ----\n", (char*)ret);

	cur.data_type = ret_type;
	if (ret_type != data_var) {
		if ((ret_type == data_number) || (ret_type == data_string)) {
			ret_str = ret;
			cur.data_len = strlen(ret_str) + 1;
			cur.data_value = malloc(cur.data_len + 1);
			if (cur.data_value == NULL)
				err(1,
				    "Could not malloc memory for arg string");
			strcpy(cur.data_value, ret_str);
		} else if (ret_type == data_byte) {
			ret_ret = ret;
			cur.data_len = ret_ret->data_len;
			cur.data_value = malloc(cur.data_len);
			if (cur.data_value == NULL)
				err(1,
				    "Could not malloc memory for byte string");
			memcpy(cur.data_value, ret_ret->data_value,
			       cur.data_len);
		} else if (ret_type == data_ref) {
			if ((cur.data_index = find_var_index(ret)) < 0)
				err(1, "Undefined variable reference");
		}
	} else {
		cur.data_index = find_var_index(ret);
		if (cur.data_index < 0)
			cur.data_index = assign_var(ret);
	}

	temp = realloc(command.returns, sizeof(*temp) * (command.nrets + 1));
	if (temp == NULL)
		err(1, "Failed to reallocate returns");
	command.returns = temp;
	memcpy(&command.returns[command.nrets], &cur, sizeof(ct_data_t));
	command.nrets++;
}

/*
 * Find the given variable name in the var array and return the i
 * return -1 if var is not found.
 */
static int
find_var_index(const char *var_name)
{
	int result;
	size_t i;

	result = -1;

	for (i = 0; i < nvars; i++) {
		if (strcmp(var_name, vars[i].name) == 0) {
			result = i;
			break;
		}
	}

	return result;
}

/*
 * Check the given function name in the given table of names, return 1 if
 * there is a match.
 */
static int check_function_table(char *function, const char *table[],
				int nfunctions)
{
	int i;

	for (i = 0; i < nfunctions; i++) {
		if ((strlen(function) == strlen(table[i])) &&
		    (strcmp(function, table[i]) == 0))
			return 1;
	}

	return 0;
}

/*
 * Compare the output from the slave against the given file and report
 * any differences.
 */
static void
compare_streams(char *filename, bool discard)
{
	char check_file[PATH_MAX], drain[100], ref, data;
	struct pollfd fds[2];
	int nfd, check_fd;
	ssize_t result;
	size_t offs;

	/*
	 * Don't prepend check path iff check file has an absolute
	 * path.
	 */
	if (filename[0] != '/') {
		if (strlcpy(check_file, check_path, sizeof(check_file))
		    >= sizeof(check_file))
			err(2, "CHECK_PATH too long");

		if (strlcat(check_file, "/", sizeof(check_file))
		    >= sizeof(check_file))
			err(2, "Could not append / to check file path");
	} else {
		check_file[0] = '\0';
	}

	if (strlcat(check_file, filename, sizeof(check_file))
	    >= sizeof(check_file))
		err(2, "Path to check file path overflowed");
	
	int create_check_file = 0;

	if((check_file_flag == (GEN_CHECK_FILE | FORCE_GEN)))
		create_check_file = 1;
	else if ((check_fd = open(check_file, O_RDONLY, 0)) < 0){ 
		if(check_file_flag & GEN_CHECK_FILE)
			create_check_file = 1;
		else
			err(2, "failed to open file %s line %zu of file %s",
				check_file, line, cur_file);
	}
	

	if(create_check_file)
		if ((check_fd = open(check_file, O_WRONLY | O_CREAT, 0644)) < 0){
			err(2, "failed to create file %s line %zu of file %s",
				check_file, line, cur_file);
	}

	fds[0].fd = check_fd;
	fds[0].events = create_check_file ? POLLOUT:POLLIN;
	fds[1].fd = master;
	fds[1].events = POLLIN;

	nfd = 2;
	/*
	 * if we have saved output then only check for data in the
	 * reference file since the slave data may already be drained.
	 */
	if (saved_output.count > 0)
		nfd = 1;

	offs = 0;
	while (poll(fds, nfd, 500) == nfd) {
		/* Read from check file if doing comparison */
		if(!create_check_file){
			if (fds[0].revents & POLLIN) {
				if ((result = read(check_fd, &ref, 1)) < 1) {
					if (result != 0) {
						err(2,
							"Bad read on file %s", check_file);
					} else {
						break;
					}
				}
			}
		}

		if (saved_output.count > 0) {
			data = saved_output.data[saved_output.readp];
			saved_output.count--;
			saved_output.readp++;
			/* run out of saved data, switch to file */
			if (saved_output.count == 0)
				nfd = 2;
		} else {
			int revent = (create_check_file == 1) ? POLLOUT:POLLIN; 
			if (fds[0].revents & revent) {
				if (read(master, &data, 1) < 1)
					err(2, "Bad read on slave pty");
			} else
				continue;
		}

		if(create_check_file){
			if ((result = write(check_fd, &data, 1)) < 1) {
				if (result != 0) {
					err(2,
						"Bad write on file %s", check_file);
				}
			}
			else
				ref = data;
		}

		if (verbose) {
			if(create_check_file)
				fprintf(stderr, "Saving reference byte 0x%x (%c)"
					" against slave byte 0x%x (%c)\n",
					ref, (ref >= ' ') ? ref : '-',
					data, (data >= ' ' )? data : '-');
			else
				fprintf(stderr, "Comparing reference byte 0x%x (%c)"
					" against slave byte 0x%x (%c)\n",
					ref, (ref >= ' ') ? ref : '-',
					data, (data >= ' ' )? data : '-');
		}

		if (!create_check_file && ref != data) {
			errx(2, "%s, %zu: refresh data from slave does "
			    "not match expected from file %s offs %zu "
			    "[reference 0x%x (%c) != slave 0x%x (%c)]",
			    cur_file, line, check_file, offs,
			    ref, (ref >= ' ') ? ref : '-',
			    data, (data >= ' ') ? data : '-');
		}

		offs++;
	}

	/*
	 * if creating a check file, there shouldn't be 
	 * anymore saved output 
	 */
	if (saved_output.count > 0){
		if(create_check_file)
			err(2, "Slave output not flushed correctly");
		else
			excess(cur_file, line, __func__, " from slave",
				&saved_output.data[saved_output.readp], saved_output.count);
	}

	/* discard any excess saved output if required */
	if (discard) {
		saved_output.count = 0;
		saved_output.readp = 0;
	}

	if (!create_check_file && (result = poll(fds, 2, 0)) != 0) {
		if (result == -1)
			err(2, "poll of file descriptors failed");

		if ((fds[1].revents & POLLIN) == POLLIN) {
			save_slave_output(true);
		} else if ((fds[0].revents & POLLIN) == POLLIN) {
			/*
			 * handle excess in file if it exists.  Poll
			 * says there is data until EOF is read.
			 * Check next read is EOF, if it is not then
			 * the file really has more data than the
			 * slave produced so flag this as a warning.
			 */
			result = read(check_fd, drain, sizeof(drain));
			if (result == -1)
				err(1, "read of data file failed");

			if (result > 0) {
				excess(check_file, 0, __func__, "", drain,
				    result);
			}
		}
	}

	close(check_fd);
}

/*
 * Pass a function call and arguments to the slave and wait for the
 * results.  The variable nresults determines how many returns we expect
 * back from the slave.  These results will be validated against the
 * expected returns or assigned to variables.
 */
static void
do_function_call(size_t nresults)
{
#define MAX_RESULTS 4
	char *p;
	int do_input;
	size_t i;
	struct pollfd fds[3];
	ct_data_t response[MAX_RESULTS], returns_count;
	assert(nresults <= MAX_RESULTS);

	do_input = check_function_table(command.function, input_functions,
	    ninput_functions);

	write_func_and_args();

	/*
	 * We should get the number of returns back here, grab it before
	 * doing input otherwise it will confuse the input poll
	 */
	read_cmd_pipe(&returns_count);
	if (returns_count.data_type != data_count)
		err(2, "expected return type of data_count but received %s",
		    enum_names[returns_count.data_type]);

	perform_delay(&delay_post_call); /* let slave catch up */

	if (verbose) {
		fprintf(stderr, "Expect %zu results from slave, slave "
		    "reported %zu\n", nresults, returns_count.data_len);
	}

	if ((no_input == false) && (do_input == 1)) {
		if (verbose) {
			fprintf(stderr, "doing input with inputstr >%s<\n",
			    input_str);
		}

		if (input_str == NULL)
			errx(2, "%s, %zu: Call to input function "
			    "but no input defined", cur_file, line);

		fds[0].fd = slvpipe[READ_PIPE];
		fds[0].events = POLLIN;
		fds[1].fd = master;
		fds[1].events = POLLOUT;
 		p = input_str;
		save_slave_output(false);
		while(*p != '\0') {
			perform_delay(&delay_spec);

			if (poll(fds, 2, 0) < 0)
				err(2, "poll failed");
			if (fds[0].revents & POLLIN) {
				warnx("%s, %zu: Slave function "
				    "returned before end of input string",
				    cur_file, line);
				break;
			}
			if ((fds[1].revents & POLLOUT) == 0)
				continue;
			if (verbose) {
				fprintf(stderr, "Writing char >%c< to slave\n",
				    *p);
			}
			if (write(master, p, 1) != 1) {
				warn("%s, %zu: Slave function write error",
				    cur_file, line);
				break;
			}
			p++;

		}
		save_slave_output(false);

		if (verbose) {
			fprintf(stderr, "Input done.\n");
		}

		/* done with the input string, free the resources */
		free(input_str);
		input_str = NULL;
	}

	if (verbose) {
		fds[0].fd = slvpipe[READ_PIPE];
		fds[0].events = POLLIN;

		fds[1].fd = slvpipe[WRITE_PIPE];
		fds[1].events = POLLOUT;

		fds[2].fd = master;
		fds[2].events = POLLIN | POLLOUT;

		i = poll(&fds[0], 3, 1000);
		fprintf(stderr, "Poll returned %zu\n", i);
		for (i = 0; i < 3; i++) {
			fprintf(stderr, "revents for fd[%zu] = 0x%x\n",
				i, fds[i].revents);
		}
	}

	/* drain any trailing output */
	save_slave_output(false);

	for (i = 0; i < returns_count.data_len; i++) {
		read_cmd_pipe(&response[i]);
	}

	/*
	 * Check for a slave error in the first return slot, if the
	 * slave errored then we may not have the number of returns we
	 * expect but in this case we should report the slave error
	 * instead of a return count mismatch.
	 */
	if ((returns_count.data_len > 0) &&
	    (response[0].data_type == data_slave_error))
		err(2, "Slave returned error: %s",
		    (const char *)response[0].data_value);

	if (returns_count.data_len != nresults)
		err(2, "Incorrect number of returns from slave, expected %zu "
		    "but received %zu", nresults, returns_count.data_len);

	if (verbose) {
		for (i = 0; i < nresults; i++) {
			if ((response[i].data_type != data_byte) &&
			    (response[i].data_type != data_err) &&
			    (response[i].data_type != data_ok))
				fprintf(stderr,
					"received response >%s< "
					"expected",
					(const char *)response[i].data_value);
			else
				fprintf(stderr, "received");

			fprintf(stderr, " data_type %s\n",
			    enum_names[command.returns[i].data_type]);
		}
	}

	for (i = 0; i < nresults; i++) {
		if (command.returns[i].data_type != data_var) {
			validate(i, &response[i]);
		} else {
			fprintf(stderr, "--- %s %d ---\n",
			 	vars[command.returns[i].data_index].name, (int)response[i].data_len);
			vars[command.returns[i].data_index].len =
				response[i].data_len;
			
			void *str = malloc(2);
			switch(response[i].data_type){
				case data_cchar:
					vars[command.returns[i].data_index].cchar =
						*((cchar_t *)response[i].data_value);
					break;
				case data_ok:
					memcpy(str, "OK", 2);
					vars[command.returns[i].data_index].value = str;
					break;
				default:
					err(2, "Incorrect return for function");
			}
			/*if(response[i].data_type == data_cchar){
				vars[command.returns[i].data_index].cchar =
					*((cchar_t *)response[i].data_value);
			}
			else{
				vars[command.returns[i].data_index].value =
					response[i].data_value;
			}*/
			vars[command.returns[i].data_index].type =
				response[i].data_type;
		}
	}

	if (verbose && (saved_output.count > 0))
		excess(cur_file, line, __func__, " from slave",
		    &saved_output.data[saved_output.readp], saved_output.count);

	init_parse_variables(0);
}

/*
 * Write the function and command arguments to the command pipe.
 */
static void
write_func_and_args(void)
{
	int i;

	if (verbose) {
		fprintf(stderr, "calling function >%s<\n", command.function);
	}

	write_cmd_pipe(command.function);
	for (i = 0; i < command.nargs; i++) {
		if (command.args[i].arg_type == data_var)
			write_cmd_pipe_args(command.args[i].arg_type,
					    &vars[command.args[i].var_index]);
		else
			write_cmd_pipe_args(command.args[i].arg_type,
					    &command.args[i]);
	}

	write_cmd_pipe(NULL); /* signal end of arguments */
}

/*
 * Initialise the command structure - if initial is non-zero then just set
 * everything to sane values otherwise free any memory that was allocated
 * when building the structure.
 */
void
init_parse_variables(int initial)
{
	int i, result;
	struct pollfd slave_pty;

	if (initial == 0) {
		free(command.function);
		for (i = 0; i < command.nrets; i++) {
			if (command.returns[i].data_type == data_number)
				free(command.returns[i].data_value);
		}
		free(command.returns);

		for (i = 0; i < command.nargs; i++) {
			if (command.args[i].arg_type != data_var)
				free(command.args[i].arg_string);
		}
		free(command.args);
	} else {
		line = 0;
		input_delay = 0;
		vars = NULL;
		nvars = 0;
		input_str = NULL;
		saved_output.allocated = 0;
		saved_output.count = 0;
		saved_output.readp = 0;
		saved_output.data = NULL;
	}

	no_input = false;
	command.function = NULL;
	command.nargs = 0;
	command.args = NULL;
	command.nrets = 0;
	command.returns = NULL;

	/*
	 * Check the slave pty for stray output from the slave, at this
	 * point we should not see any data as it should have been
	 * consumed by the test functions.  If we see data then we have
	 * either a bug or are not handling an output generating function
	 * correctly.
	 */
	slave_pty.fd = master;
	slave_pty.events = POLLIN;
	result = poll(&slave_pty, 1, 0);

	if (result < 0)
		err(2, "Poll of slave pty failed");
	else if (result > 0)
		warnx("%s, %zu: Unexpected data from slave", cur_file, line);
}

/*
 * Validate the response against the expected return.  The variable
 * i is the i into the rets array in command.
 */
static void
validate(int i, void *data)
{
	char *response;
	ct_data_t *byte_response;

	byte_response = data;
	if ((command.returns[i].data_type != data_byte) &&
	    (command.returns[i].data_type != data_err) &&
	    (command.returns[i].data_type != data_ok)) {
		if ((byte_response->data_type == data_byte) ||
		    (byte_response->data_type == data_err) ||
		    (byte_response->data_type == data_ok))
			err(1, "%s: expecting type %s, received type %s"
			    " at line %zu of file %s", __func__,
			    enum_names[command.returns[i].data_type],
			    enum_names[byte_response->data_type],
			    line, cur_file);

		response = byte_response->data_value;
	}

	switch (command.returns[i].data_type) {
	case data_err:
		validate_type(data_err, byte_response, 0);
		break;

	case data_ok:
		validate_type(data_ok, byte_response, 0);
		break;

	case data_null:
		validate_return("NULL", response, 0);
		break;

	case data_nonnull:
		validate_return("NULL", response, 1);
		break;

	case data_string:
	case data_number:
		validate_return(command.returns[i].data_value,
				response, 0);
		break;

	case data_ref:
		validate_reference(i, response);
		break;

	case data_byte:
		validate_byte(&command.returns[i], byte_response, 0);
		break;

	default:
		err(1, "Malformed statement at line %zu of file %s",
		    line, cur_file);
		break;
	}
}

/*
 * Validate the return against the contents of a variable.
 */
static void
validate_reference(int i, void *data)
{
	char *response;
	ct_data_t *byte_response;
	var_t *varp;

	varp = &vars[command.returns[i].data_index];

	byte_response = data;
	if (command.returns[i].data_type != data_byte)
		response = data;

	if (verbose) {
		fprintf(stderr,
		    "%s: return type of %s, value %s \n", __func__,
		    enum_names[varp->type],
		    (const char *)varp->value);
	}

	switch (varp->type) {
	case data_string:
	case data_number:
		validate_return(varp->value, response, 0);
		break;

	case data_byte:
		validate_byte(varp->value, byte_response, 0);
		break;

	default:
		err(1,
		    "Invalid return type for reference at line %zu of file %s",
		    line, cur_file);
		break;
	}
}

/*
 * Validate the return type against the expected type, throw an error
 * if they don't match.
 */
static void
validate_type(data_enum_t expected, ct_data_t *value, int check)
{
	if (((check == 0) && (expected != value->data_type)) ||
	    ((check == 1) && (expected == value->data_type)))
		err(1, "Validate expected type %s %s %s line %zu of file %s",
		    enum_names[expected],
		    (check == 0)? "matching" : "not matching",
		    enum_names[value->data_type], line, cur_file);

	if (verbose) {
		fprintf(stderr, "Validate expected type %s %s %s line %zu"
		    " of file %s\n",
		    enum_names[expected],
		    (check == 0)? "matching" : "not matching",
		    enum_names[value->data_type], line, cur_file);
	}
}

/*
 * Validate the return value against the expected value, throw an error
 * if they don't match.
 */
static void
validate_return(const char *expected, const char *value, int check)
{
	if (((check == 0) && strcmp(expected, value) != 0) ||
	    ((check == 1) && strcmp(expected, value) == 0))
		errx(1, "Validate expected >%s< %s >%s< line %zu of file %s",
		    expected,
		    (check == 0)? "matching" : "not matching", value,
		    line, cur_file);
	if (verbose) {
		fprintf(stderr, "Validated expected value >%s< %s >%s< "
		    "at line %zu of file %s\n", expected,
		    (check == 0)? "matches" : "does not match",
		    value, line, cur_file);
	}
}

/*
 * Validate the return value against the expected value, throw an error
 * if they don't match expectations.
 */
static void
validate_byte(ct_data_t *expected, ct_data_t *value, int check)
{
	char *ch;
	size_t i;

	if (verbose) {
		ch = value->data_value;
		fprintf(stderr, "checking returned byte stream: ");
		for (i = 0; i < value->data_len; i++)
			fprintf(stderr, "%s0x%x", (i != 0)? ", " : "", ch[i]);
		fprintf(stderr, "\n");

		fprintf(stderr, "%s byte stream: ",
			(check == 0)? "matches" : "does not match");
		ch = (char *) expected->data_value;
		for (i = 0; i < expected->data_len; i++)
			fprintf(stderr, "%s0x%x", (i != 0)? ", " : "", ch[i]);
		fprintf(stderr, "\n");
	}

	/*
	 * No chance of a match if lengths differ...
	 */
	if ((check == 0) && (expected->data_len != value->data_len))
	    errx(1, "Byte validation failed, length mismatch, expected %zu,"
		"received %zu", expected->data_len, value->data_len);

	/*
	 * If check is 0 then we want to throw an error IFF the byte streams
	 * do not match, if check is 1 then throw an error if the byte
	 * streams match.
	 */
	if (((check == 0) && memcmp(expected->data_value, value->data_value,
				    value->data_len) != 0) ||
	    ((check == 1) && (expected->data_len == value->data_len) &&
	     memcmp(expected->data_value, value->data_value,
		    value->data_len) == 0))
		errx(1, "Validate expected %s byte stream at line %zu"
		    "of file %s",
		    (check == 0)? "matching" : "not matching", line, cur_file);
	if (verbose) {
		fprintf(stderr, "Validated expected %s byte stream "
		    "at line %zu of file %s\n",
		    (check == 0)? "matching" : "not matching",
		    line, cur_file);
	}
}

/*
 * cchar_t
 * Validate the return value against the expected value, throw an error
 * if they don't match expectations.
 */
static void
validate_cchar(var_t *expected, var_t *value, int check)
{
	/*
	 * If check is 0 then we want to throw an error IFF the byte streams
	 * do not match, if check is 1 then throw an error if the byte
	 * streams match.
	 */
	if (((check == 0) && memcmp((void *)&expected->cchar, (void *)&value->cchar,
				    sizeof(cchar_t)) != 0) ||
	    ((check == 1) && memcmp((void *)&expected->cchar, (void *)&value->cchar, 
		 sizeof(cchar_t)) == 0))
		errx(1, "Validate expected %s cchar at line %zu"
		    "of file %s",
		    (check == 0)? "matching" : "not matching", line, cur_file);
	if (verbose) {
		fprintf(stderr, "Validated expected %s cchar "
		    "at line %zu of file %s\n",
		    (check == 0)? "matching" : "not matching",
		    line, cur_file);
	}
}

/*
 * Validate the variable at i against the expected value, throw an
 * error if they don't match, if check is non-zero then the match is
 * negated.
 */
static void
validate_variable(int ret, data_enum_t type, const void *value, int i,
    int check)
{
	ct_data_t *retval;
	var_t *varptr;

	retval = &command.returns[ret];
	varptr = &vars[command.returns[ret].data_index];

	if (varptr->value == NULL)
		err(1, "Variable %s has no value assigned to it", varptr->name);


	if (varptr->type != type)
		err(1, "Variable %s is not the expected type", varptr->name);

	if (type != data_byte) {
		if ((((check == 0) && strcmp(value, varptr->value) != 0))
		    || ((check == 1) && strcmp(value, varptr->value) == 0))
			err(1, "Variable %s contains %s instead of %s"
			    " value %s at line %zu of file %s",
			    varptr->name, (const char *)varptr->value,
			    (check == 0)? "expected" : "not matching",
			    (const char *)value,
			    line, cur_file);
		if (verbose) {
			fprintf(stderr, "Variable %s contains %s value "
			    "%s at line %zu of file %s\n",
			    varptr->name,
			    (check == 0)? "expected" : "not matching",
			    (const char *)varptr->value, line, cur_file);
		}
	} else {
		if ((check == 0) && (retval->data_len != varptr->len))
			err(1, "Byte validation failed, length mismatch");

		/*
		 * If check is 0 then we want to throw an error IFF
		 * the byte streams do not match, if check is 1 then
		 * throw an error if the byte streams match.
		 */
		if (((check == 0) && memcmp(retval->data_value, varptr->value,
					    varptr->len) != 0) ||
		    ((check == 1) && (retval->data_len == varptr->len) &&
		     memcmp(retval->data_value, varptr->value,
			    varptr->len) == 0))
			err(1, "Validate expected %s byte stream at line %zu"
			    " of file %s",
			    (check == 0)? "matching" : "not matching",
			    line, cur_file);
		if (verbose) {
			fprintf(stderr, "Validated expected %s byte stream "
			    "at line %zu of file %s\n",
			    (check == 0)? "matching" : "not matching",
			    line, cur_file);
		}
	}
}

/*
 * Write a string to the command pipe - we feed the number of bytes coming
 * down first to allow storage allocation and then follow up with the data.
 * If cmd is NULL then feed a -1 down the pipe to say the end of the args.
 */
static void
write_cmd_pipe(char *cmd)
{
	args_t arg;
	size_t len;

	if (cmd == NULL)
		len = 0;
	else
		len = strlen(cmd);

	arg.arg_type = data_static;
	arg.arg_len = len;
	arg.arg_string = cmd;
	write_cmd_pipe_args(arg.arg_type, &arg);

}

static void
write_cmd_pipe_args(data_enum_t type, void *data)
{
	var_t *var_data;
	args_t *arg_data;
	int len, send_type;
	void *cmd;

	arg_data = data;
	switch (type) {
	case data_var:
		var_data = data;
		len = var_data->len;
		cmd = var_data->value;

		switch (var_data->type) {
		case data_byte:
			send_type = data_byte;
			break;

		case data_cchar:
			send_type = data_cchar;
			cmd = (void *) &var_data->cchar;
			len = sizeof(cchar_t);
			break;

		default:
			send_type = data_string;
			break;
		}
		break;

	case data_null:
		send_type = data_null;
		len = 0;
		break;

	default:
		if ((arg_data->arg_len == 0) && (arg_data->arg_string == NULL))
			len = -1;
		else
			len = arg_data->arg_len;
		cmd = arg_data->arg_string;
		if (type == data_byte)
			send_type = data_byte;
		else
			send_type = data_string;
	}

	if (verbose) {
		fprintf(stderr, "Writing type %s to command pipe\n",
		    enum_names[send_type]);
	}

	if (write(cmdpipe[WRITE_PIPE], &send_type, sizeof(int)) < 0)
		err(1, "command pipe write for type failed");

	if (verbose) {
		if (send_type == data_cchar)
			fprintf(stderr,
			    "Writing cchar to command pipe\n");
		else
			fprintf(stderr,
			    "Writing length %d to command pipe\n", len);
	}

	if (write(cmdpipe[WRITE_PIPE], &len, sizeof(int)) < 0)
		err(1, "command pipe write for length failed");

	if (len > 0) {
		if (verbose) {
			fprintf(stderr, "Writing data >%s< to command pipe\n",
			    (const char *)cmd);
		}
		if (write(cmdpipe[WRITE_PIPE], cmd, len) < 0)
			err(1, "command pipe write of data failed");
	}
}

/*
 * Read a response from the command pipe, first we will receive the
 * length of the response then the actual data.
 */
static void
read_cmd_pipe(ct_data_t *response)
{
	int len, type;
	struct pollfd rfd[2];
	char *str;

	/*
	 * Check if there is data to read - just in case slave has died, we
	 * don't want to block on the read and just hang.  We also check
	 * output from the slave because the slave may be blocked waiting
	 * for a flush on its stdout.
	 */
	rfd[0].fd = slvpipe[READ_PIPE];
	rfd[0].events = POLLIN;
	rfd[1].fd = master;
	rfd[1].events = POLLIN;

	do {
		if (poll(rfd, 2, 4000) == 0)
			errx(2, "%s, %zu: Command pipe read timeout",
			    cur_file, line);

		if ((rfd[1].revents & POLLIN) == POLLIN) {
			if (verbose) {
				fprintf(stderr,
				    "draining output from slave\n");
			}
			save_slave_output(false);
		}
	}
	while((rfd[1].revents & POLLIN) == POLLIN);

	if (read(slvpipe[READ_PIPE], &type, sizeof(int)) < 0)
		err(1, "command pipe read for type failed");
	response->data_type = type;

	if ((type != data_ok) && (type != data_err) && (type != data_count)) {
		if (read(slvpipe[READ_PIPE], &len, sizeof(int)) < 0)
			err(1, "command pipe read for length failed");
		response->data_len = len;

		if (verbose) {
			fprintf(stderr,
			    "Reading %d bytes from command pipe\n", len);
		}

		if ((response->data_value = malloc(len + 1)) == NULL)
			err(1, "Failed to alloc memory for cmd pipe read");

		if (read(slvpipe[READ_PIPE], response->data_value, len) < 0)
			err(1, "command pipe read of data failed");

		if (response->data_type != data_byte) {
			str = response->data_value;
			str[len] = '\0';

			if (verbose) {
				fprintf(stderr, "Read data >%s< from pipe\n",
				    (const char *)response->data_value);
			}
		}
	} else {
		response->data_value = NULL;
		if (type == data_count) {
			if (read(slvpipe[READ_PIPE], &len, sizeof(int)) < 0)
				err(1, "command pipe read for number of "
				       "returns failed");
			response->data_len = len;
		}

		if (verbose) {
			fprintf(stderr, "Read type %s from pipe\n",
			    enum_names[type]);
		}
	}
}

/*
 * Check for writes from the slave on the pty, save the output into a
 * buffer for later checking if discard is false.
 */
#define MAX_DRAIN 256

static void
save_slave_output(bool discard)
{
	char *new_data, drain[MAX_DRAIN];
	size_t to_allocate;
	ssize_t result;
	size_t i;

	result = 0;
	for (;;) {
		if (result == -1)
			err(2, "poll of slave pty failed");
		result = MAX_DRAIN;
		if ((result = read(master, drain, result)) < 0) {
			if (errno == EAGAIN)
				break;
			else
				err(2, "draining slave pty failed");
		}
		if (result == 0)
			abort();

		if (!discard) {
			if ((size_t)result >
			    (saved_output.allocated - saved_output.count)) {
				to_allocate = 1024 * ((result / 1024) + 1);

				if ((new_data = realloc(saved_output.data,
					saved_output.allocated + to_allocate))
				    == NULL)
					err(2, "Realloc of saved_output failed");
				saved_output.data = new_data;
				saved_output.allocated += to_allocate;
			}

			if (verbose) {
				fprintf(stderr, "count = %zu, "
				    "allocated = %zu\n", saved_output.count,
				    saved_output.allocated);
				for (i = 0; i < (size_t)result; i++) {
					fprintf(stderr, "Saving slave output "
					    "at %zu: 0x%x (%c)\n",
					    saved_output.count + i, drain[i],
					    (drain[i] >= ' ')? drain[i] : '-');
				}
			}

			memcpy(&saved_output.data[saved_output.count], drain,
			       result);
			saved_output.count += result;

			if (verbose) {
				fprintf(stderr, "count = %zu, "
				    "allocated = %zu\n", saved_output.count,
				    saved_output.allocated);
			}
		} else {
			if (verbose) {
				for (i = 0; i < (size_t)result; i++) {
					fprintf(stderr, "Discarding slave "
					    "output 0x%x (%c)\n",
					    drain[i],
					    (drain[i] >= ' ')? drain[i] : '-');
				}
			}
		}
	}
}

static void
yyerror(const char *msg)
{
	warnx("%s in line %zu of file %s", msg, line, cur_file);
}
#line 2119 "testlang_parse.c"

/* For use in generated program */
#define yydepth (int)(yystack.s_mark - yystack.s_base)
#if YYBTYACC
#define yytrial (yyps->save)
#endif /* YYBTYACC */

#if YYDEBUG
#include <stdio.h>	/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    YYINT *newss;
    YYSTYPE *newvs;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE *newps;
#endif

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return YYENOMEM;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = (int) (data->s_mark - data->s_base);
    newss = (YYINT *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return YYENOMEM;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return YYENOMEM;

    data->l_base = newvs;
    data->l_mark = newvs + i;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    newps = (YYLTYPE *)realloc(data->p_base, newsize * sizeof(*newps));
    if (newps == 0)
        return YYENOMEM;

    data->p_base = newps;
    data->p_mark = newps + i;
#endif

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;

#if YYDEBUG
    if (yydebug)
        fprintf(stderr, "%sdebug: stack size increased to %d\n", YYPREFIX, newsize);
#endif
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    free(data->p_base);
#endif
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif /* YYPURE || defined(YY_NO_LEAKS) */
#if YYBTYACC

static YYParseState *
yyNewState(unsigned size)
{
    YYParseState *p = (YYParseState *) malloc(sizeof(YYParseState));
    if (p == NULL) return NULL;

    p->yystack.stacksize = size;
    if (size == 0)
    {
        p->yystack.s_base = NULL;
        p->yystack.l_base = NULL;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        p->yystack.p_base = NULL;
#endif
        return p;
    }
    p->yystack.s_base    = (YYINT *) malloc(size * sizeof(YYINT));
    if (p->yystack.s_base == NULL) return NULL;
    p->yystack.l_base    = (YYSTYPE *) malloc(size * sizeof(YYSTYPE));
    if (p->yystack.l_base == NULL) return NULL;
    memset(p->yystack.l_base, 0, size * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    p->yystack.p_base    = (YYLTYPE *) malloc(size * sizeof(YYLTYPE));
    if (p->yystack.p_base == NULL) return NULL;
    memset(p->yystack.p_base, 0, size * sizeof(YYLTYPE));
#endif

    return p;
}

static void
yyFreeState(YYParseState *p)
{
    yyfreestack(&p->yystack);
    free(p);
}
#endif /* YYBTYACC */

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab
#if YYBTYACC
#define YYVALID        do { if (yyps->save)            goto yyvalid; } while(0)
#define YYVALID_NESTED do { if (yyps->save && \
                                yyps->save->save == 0) goto yyvalid; } while(0)
#endif /* YYBTYACC */

int
YYPARSE_DECL()
{
    int yym, yyn, yystate, yyresult;
#if YYBTYACC
    int yynewerrflag;
    YYParseState *yyerrctx = NULL;
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE  yyerror_loc_range[3]; /* position of error start/end (0 unused) */
#endif
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
    if (yydebug)
        fprintf(stderr, "%sdebug[<# of symbols on state stack>]\n", YYPREFIX);
#endif
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    memset(yyerror_loc_range, 0, sizeof(yyerror_loc_range));
#endif

#if YYBTYACC
    yyps = yyNewState(0); if (yyps == 0) goto yyenomem;
    yyps->save = 0;
#endif /* YYBTYACC */
    yym = 0;
    yyn = 0;
    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark = yystack.p_base;
#endif
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
#if YYBTYACC
        do {
        if (yylvp < yylve)
        {
            /* we're currently re-reading tokens */
            yylval = *yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylloc = *yylpp++;
#endif
            yychar = *yylexp++;
            break;
        }
        if (yyps->save)
        {
            /* in trial mode; save scanner results for future parse attempts */
            if (yylvp == yylvlim)
            {   /* Enlarge lexical value queue */
                size_t p = (size_t) (yylvp - yylvals);
                size_t s = (size_t) (yylvlim - yylvals);

                s += YYLVQUEUEGROWTH;
                if ((yylexemes = (YYINT *)realloc(yylexemes, s * sizeof(YYINT))) == NULL) goto yyenomem;
                if ((yylvals   = (YYSTYPE *)realloc(yylvals, s * sizeof(YYSTYPE))) == NULL) goto yyenomem;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                if ((yylpsns   = (YYLTYPE *)realloc(yylpsns, s * sizeof(YYLTYPE))) == NULL) goto yyenomem;
#endif
                yylvp   = yylve = yylvals + p;
                yylvlim = yylvals + s;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp   = yylpe = yylpsns + p;
                yylplim = yylpsns + s;
#endif
                yylexp  = yylexemes + p;
            }
            *yylexp = (YYINT) YYLEX;
            *yylvp++ = yylval;
            yylve++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            *yylpp++ = yylloc;
            yylpe++;
#endif
            yychar = *yylexp++;
            break;
        }
        /* normal operation, no conflict encountered */
#endif /* YYBTYACC */
        yychar = YYLEX;
#if YYBTYACC
        } while (0);
#endif /* YYBTYACC */
        if (yychar < 0) yychar = YYEOF;
#if YYDEBUG
        if (yydebug)
        {
            if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
            fprintf(stderr, "%s[%d]: state %d, reading token %d (%s)",
                            YYDEBUGSTR, yydepth, yystate, yychar, yys);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
            if (!yytrial)
#endif /* YYBTYACC */
                fprintf(stderr, " <%s>", YYSTYPE_TOSTRING(yychar, yylval));
#endif
            fputc('\n', stderr);
        }
#endif
    }
#if YYBTYACC

    /* Do we have a conflict? */
    if (((yyn = yycindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
        yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
        YYINT ctry;

        if (yypath)
        {
            YYParseState *save;
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%s[%d]: CONFLICT in state %d: following successful trial parse\n",
                                YYDEBUGSTR, yydepth, yystate);
#endif
            /* Switch to the next conflict context */
            save = yypath;
            yypath = save->save;
            save->save = NULL;
            ctry = save->ctry;
            if (save->state != yystate) YYABORT;
            yyFreeState(save);

        }
        else
        {

            /* Unresolved conflict - start/continue trial parse */
            YYParseState *save;
#if YYDEBUG
            if (yydebug)
            {
                fprintf(stderr, "%s[%d]: CONFLICT in state %d. ", YYDEBUGSTR, yydepth, yystate);
                if (yyps->save)
                    fputs("ALREADY in conflict, continuing trial parse.\n", stderr);
                else
                    fputs("Starting trial parse.\n", stderr);
            }
#endif
            save                  = yyNewState((unsigned)(yystack.s_mark - yystack.s_base + 1));
            if (save == NULL) goto yyenomem;
            save->save            = yyps->save;
            save->state           = yystate;
            save->errflag         = yyerrflag;
            save->yystack.s_mark  = save->yystack.s_base + (yystack.s_mark - yystack.s_base);
            memcpy (save->yystack.s_base, yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            save->yystack.l_mark  = save->yystack.l_base + (yystack.l_mark - yystack.l_base);
            memcpy (save->yystack.l_base, yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            save->yystack.p_mark  = save->yystack.p_base + (yystack.p_mark - yystack.p_base);
            memcpy (save->yystack.p_base, yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            ctry                  = yytable[yyn];
            if (yyctable[ctry] == -1)
            {
#if YYDEBUG
                if (yydebug && yychar >= YYEOF)
                    fprintf(stderr, "%s[%d]: backtracking 1 token\n", YYDEBUGSTR, yydepth);
#endif
                ctry++;
            }
            save->ctry = ctry;
            if (yyps->save == NULL)
            {
                /* If this is a first conflict in the stack, start saving lexemes */
                if (!yylexemes)
                {
                    yylexemes = (YYINT *) malloc((YYLVQUEUEGROWTH) * sizeof(YYINT));
                    if (yylexemes == NULL) goto yyenomem;
                    yylvals   = (YYSTYPE *) malloc((YYLVQUEUEGROWTH) * sizeof(YYSTYPE));
                    if (yylvals == NULL) goto yyenomem;
                    yylvlim   = yylvals + YYLVQUEUEGROWTH;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpsns   = (YYLTYPE *) malloc((YYLVQUEUEGROWTH) * sizeof(YYLTYPE));
                    if (yylpsns == NULL) goto yyenomem;
                    yylplim   = yylpsns + YYLVQUEUEGROWTH;
#endif
                }
                if (yylvp == yylve)
                {
                    yylvp  = yylve = yylvals;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpp  = yylpe = yylpsns;
#endif
                    yylexp = yylexemes;
                    if (yychar >= YYEOF)
                    {
                        *yylve++ = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                        *yylpe++ = yylloc;
#endif
                        *yylexp  = (YYINT) yychar;
                        yychar   = YYEMPTY;
                    }
                }
            }
            if (yychar >= YYEOF)
            {
                yylvp--;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp--;
#endif
                yylexp--;
                yychar = YYEMPTY;
            }
            save->lexeme = (int) (yylvp - yylvals);
            yyps->save   = save;
        }
        if (yytable[yyn] == ctry)
        {
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%s[%d]: state %d, shifting to state %d\n",
                                YYDEBUGSTR, yydepth, yystate, yyctable[ctry]);
#endif
            if (yychar < 0)
            {
                yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp++;
#endif
                yylexp++;
            }
            if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM)
                goto yyoverflow;
            yystate = yyctable[ctry];
            *++yystack.s_mark = (YYINT) yystate;
            *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            *++yystack.p_mark = yylloc;
#endif
            yychar  = YYEMPTY;
            if (yyerrflag > 0) --yyerrflag;
            goto yyloop;
        }
        else
        {
            yyn = yyctable[ctry];
            goto yyreduce;
        }
    } /* End of code dealing with conflicts */
#endif /* YYBTYACC */
    if (((yyn = yysindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
#if YYDEBUG
        if (yydebug)
            fprintf(stderr, "%s[%d]: state %d, shifting to state %d\n",
                            YYDEBUGSTR, yydepth, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        *++yystack.p_mark = yylloc;
#endif
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if (((yyn = yyrindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag != 0) goto yyinrecovery;
#if YYBTYACC

    yynewerrflag = 1;
    goto yyerrhandler;
    goto yyerrlab; /* redundant goto avoids 'unused label' warning */

yyerrlab:
    /* explicit YYERROR from an action -- pop the rhs of the rule reduced
     * before looking for error recovery */
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark -= yym;
#endif

    yynewerrflag = 0;
yyerrhandler:
    while (yyps->save)
    {
        int ctry;
        YYParseState *save = yyps->save;
#if YYDEBUG
        if (yydebug)
            fprintf(stderr, "%s[%d]: ERROR in state %d, CONFLICT BACKTRACKING to state %d, %d tokens\n",
                            YYDEBUGSTR, yydepth, yystate, yyps->save->state,
                    (int)(yylvp - yylvals - yyps->save->lexeme));
#endif
        /* Memorize most forward-looking error state in case it's really an error. */
        if (yyerrctx == NULL || yyerrctx->lexeme < yylvp - yylvals)
        {
            /* Free old saved error context state */
            if (yyerrctx) yyFreeState(yyerrctx);
            /* Create and fill out new saved error context state */
            yyerrctx                 = yyNewState((unsigned)(yystack.s_mark - yystack.s_base + 1));
            if (yyerrctx == NULL) goto yyenomem;
            yyerrctx->save           = yyps->save;
            yyerrctx->state          = yystate;
            yyerrctx->errflag        = yyerrflag;
            yyerrctx->yystack.s_mark = yyerrctx->yystack.s_base + (yystack.s_mark - yystack.s_base);
            memcpy (yyerrctx->yystack.s_base, yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            yyerrctx->yystack.l_mark = yyerrctx->yystack.l_base + (yystack.l_mark - yystack.l_base);
            memcpy (yyerrctx->yystack.l_base, yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yyerrctx->yystack.p_mark = yyerrctx->yystack.p_base + (yystack.p_mark - yystack.p_base);
            memcpy (yyerrctx->yystack.p_base, yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            yyerrctx->lexeme         = (int) (yylvp - yylvals);
        }
        yylvp          = yylvals   + save->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        yylpp          = yylpsns   + save->lexeme;
#endif
        yylexp         = yylexemes + save->lexeme;
        yychar         = YYEMPTY;
        yystack.s_mark = yystack.s_base + (save->yystack.s_mark - save->yystack.s_base);
        memcpy (yystack.s_base, save->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
        yystack.l_mark = yystack.l_base + (save->yystack.l_mark - save->yystack.l_base);
        memcpy (yystack.l_base, save->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        yystack.p_mark = yystack.p_base + (save->yystack.p_mark - save->yystack.p_base);
        memcpy (yystack.p_base, save->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
        ctry           = ++save->ctry;
        yystate        = save->state;
        /* We tried shift, try reduce now */
        if ((yyn = yyctable[ctry]) >= 0) goto yyreduce;
        yyps->save     = save->save;
        save->save     = NULL;
        yyFreeState(save);

        /* Nothing left on the stack -- error */
        if (!yyps->save)
        {
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%sdebug[%d,trial]: trial parse FAILED, entering ERROR mode\n",
                                YYPREFIX, yydepth);
#endif
            /* Restore state as it was in the most forward-advanced error */
            yylvp          = yylvals   + yyerrctx->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylpp          = yylpsns   + yyerrctx->lexeme;
#endif
            yylexp         = yylexemes + yyerrctx->lexeme;
            yychar         = yylexp[-1];
            yylval         = yylvp[-1];
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylloc         = yylpp[-1];
#endif
            yystack.s_mark = yystack.s_base + (yyerrctx->yystack.s_mark - yyerrctx->yystack.s_base);
            memcpy (yystack.s_base, yyerrctx->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            yystack.l_mark = yystack.l_base + (yyerrctx->yystack.l_mark - yyerrctx->yystack.l_base);
            memcpy (yystack.l_base, yyerrctx->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yystack.p_mark = yystack.p_base + (yyerrctx->yystack.p_mark - yyerrctx->yystack.p_base);
            memcpy (yystack.p_base, yyerrctx->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            yystate        = yyerrctx->state;
            yyFreeState(yyerrctx);
            yyerrctx       = NULL;
        }
        yynewerrflag = 1;
    }
    if (yynewerrflag == 0) goto yyinrecovery;
#endif /* YYBTYACC */

    YYERROR_CALL("syntax error");
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yyerror_loc_range[1] = yylloc; /* lookahead position is error start position */
#endif

#if !YYBTYACC
    goto yyerrlab; /* redundant goto avoids 'unused label' warning */
yyerrlab:
#endif
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if (((yyn = yysindex[*yystack.s_mark]) != 0) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    fprintf(stderr, "%s[%d]: state %d, error recovery shifting to state %d\n",
                                    YYDEBUGSTR, yydepth, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                /* lookahead position is error end position */
                yyerror_loc_range[2] = yylloc;
                YYLLOC_DEFAULT(yyloc, yyerror_loc_range, 2); /* position of error span */
                *++yystack.p_mark = yyloc;
#endif
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    fprintf(stderr, "%s[%d]: error recovery discarding state %d\n",
                                    YYDEBUGSTR, yydepth, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                /* the current TOS position is the error start position */
                yyerror_loc_range[1] = *yystack.p_mark;
#endif
#if defined(YYDESTRUCT_CALL)
#if YYBTYACC
                if (!yytrial)
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    YYDESTRUCT_CALL("error: discarding state",
                                    yystos[*yystack.s_mark], yystack.l_mark, yystack.p_mark);
#else
                    YYDESTRUCT_CALL("error: discarding state",
                                    yystos[*yystack.s_mark], yystack.l_mark);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#endif /* defined(YYDESTRUCT_CALL) */
                --yystack.s_mark;
                --yystack.l_mark;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                --yystack.p_mark;
#endif
            }
        }
    }
    else
    {
        if (yychar == YYEOF) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
            fprintf(stderr, "%s[%d]: state %d, error recovery discarding token %d (%s)\n",
                            YYDEBUGSTR, yydepth, yystate, yychar, yys);
        }
#endif
#if defined(YYDESTRUCT_CALL)
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            YYDESTRUCT_CALL("error: discarding token", yychar, &yylval, &yylloc);
#else
            YYDESTRUCT_CALL("error: discarding token", yychar, &yylval);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#endif /* defined(YYDESTRUCT_CALL) */
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
    yym = yylen[yyn];
#if YYDEBUG
    if (yydebug)
    {
        fprintf(stderr, "%s[%d]: state %d, reducing by rule %d (%s)",
                        YYDEBUGSTR, yydepth, yystate, yyn, yyrule[yyn]);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
            if (yym > 0)
            {
                int i;
                fputc('<', stderr);
                for (i = yym; i > 0; i--)
                {
                    if (i != yym) fputs(", ", stderr);
                    fputs(YYSTYPE_TOSTRING(yystos[yystack.s_mark[1-i]],
                                           yystack.l_mark[1-i]), stderr);
                }
                fputc('>', stderr);
            }
#endif
        fputc('\n', stderr);
    }
#endif
    if (yym > 0)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)

    /* Perform position reduction */
    memset(&yyloc, 0, sizeof(yyloc));
#if YYBTYACC
    if (!yytrial)
#endif /* YYBTYACC */
    {
        YYLLOC_DEFAULT(yyloc, &yystack.p_mark[-yym], yym);
        /* just in case YYERROR is invoked within the action, save
           the start of the rhs as the error start position */
        yyerror_loc_range[1] = yystack.p_mark[1-yym];
    }
#endif

    switch (yyn)
    {
case 15:
#line 209 "testlang_parse.y"
	{set_var(data_number, yystack.l_mark[-1].string, yystack.l_mark[0].string);}
break;
case 17:
#line 210 "testlang_parse.y"
	{set_var(data_number, yystack.l_mark[-3].string, yystack.l_mark[-1].string);}
break;
case 19:
#line 211 "testlang_parse.y"
	{set_var(data_string, yystack.l_mark[-1].string, yystack.l_mark[0].string);}
break;
case 21:
#line 212 "testlang_parse.y"
	{set_var(data_byte, yystack.l_mark[-1].string, yystack.l_mark[0].retval);}
break;
case 23:
#line 216 "testlang_parse.y"
	{
				set_cchar(yystack.l_mark[-3].string, yystack.l_mark[-2].string);
			}
break;
case 25:
#line 224 "testlang_parse.y"
	{ yyval.string = yystack.l_mark[-1].string; }
break;
case 26:
#line 226 "testlang_parse.y"
	{ yyval.string = get_numeric_var(yystack.l_mark[0].string); }
break;
case 27:
#line 230 "testlang_parse.y"
	{ add_to_vals(data_number, yystack.l_mark[0].string); }
break;
case 29:
#line 233 "testlang_parse.y"
	{ add_to_vals(data_var, yystack.l_mark[0].string); }
break;
case 30:
#line 235 "testlang_parse.y"
	{ add_to_vals(data_string, yystack.l_mark[0].string); }
break;
case 31:
#line 237 "testlang_parse.y"
	{ add_to_vals(data_byte, yystack.l_mark[0].retval); }
break;
case 32:
#line 240 "testlang_parse.y"
	{
	do_function_call(1);
}
break;
case 33:
#line 245 "testlang_parse.y"
	{
	do_function_call(2);
}
break;
case 34:
#line 250 "testlang_parse.y"
	{
	do_function_call(3);
}
break;
case 35:
#line 255 "testlang_parse.y"
	{
	do_function_call(4);
 }
break;
case 36:
#line 260 "testlang_parse.y"
	{
	ct_data_t retvar;
	var_t *vptr;
	/* This if condition is unsatisfiable (useless check). Data_index will never be -1. */
	if (command.returns[0].data_index == -1)
		err(1, "Undefined variable in check statement, line %zu"
		    " of file %s", line, cur_file);

	if (verbose) {
		fprintf(stderr, "Checking contents of variable %s for %s\n",
		    vars[command.returns[0].data_index].name,
		    enum_names[command.returns[1].data_type]);
	}

	if(command.returns[1].data_type == data_var 
		&& vars[command.returns[1].data_index].type != 0){
		command.returns[1].data_type = vars[command.returns[1].data_index].type;
		command.returns[1].data_value = vars[command.returns[1].data_index].value;
		command.returns[1].data_len = vars[command.returns[1].data_index].len;
		command.returns[1].data_index = command.returns[1].data_index;
	}

	/* fprintf(stderr, "==== %d ====\n", vars[command.returns[0].data_index].cchar.vals[0]);*/

	 /*if (((command.returns[1].data_type == data_byte) &&
	     (vars[command.returns[0].data_index].type != data_byte)) ||
	    vars[command.returns[0].data_index].type != data_string) */
	if(command.returns[1].data_type 
		!= vars[command.returns[0].data_index].type)
		err(1, "Var type %s (%d) does not match return type %s (%d)",
		    enum_names[
		    vars[command.returns[0].data_index].type],
		    vars[command.returns[0].data_index].type,
		    enum_names[command.returns[1].data_type],
		    command.returns[1].data_type); 

	switch (command.returns[1].data_type) {
	case data_err:
		validate_variable(0, data_err, "ERR",
				  command.returns[0].data_index, 0);
		break;

	case data_ok:
		validate_variable(0, data_ok, "OK",
				  command.returns[0].data_index, 0);
		break;

	case data_null:
		validate_variable(0, data_string, "NULL",
				  command.returns[0].data_index, 0);
		break;

	case data_nonnull:
		validate_variable(0, data_string, "NULL",
				  command.returns[0].data_index, 1);
		break;

	case data_string:
	case data_number:
		if (verbose) {
			fprintf(stderr, " %s == returned %s\n",
			    (const char *)command.returns[1].data_value,
			    (const char *)
			    vars[command.returns[0].data_index].value);
		}
		validate_variable(0, data_string,
		    command.returns[1].data_value,
		    command.returns[0].data_index, 0);
		break;

	case data_byte:
		vptr = &vars[command.returns[0].data_index];
		retvar.data_len = vptr->len;
		retvar.data_type = vptr->type;
		retvar.data_value = vptr->value;
		validate_byte(&retvar, &command.returns[1], 0);
		break;
	
	case data_cchar:
		validate_cchar(&vars[command.returns[0].data_index],
				  &vars[command.returns[1].data_index], 0);
		break;

	default:
		err(1, "Malformed check statement at line %zu "
		    "of file %s", line, cur_file);
		break;
	}

	init_parse_variables(0);
 }
break;
case 37:
#line 353 "testlang_parse.y"
	{
	/* set the inter-character delay */
	if (sscanf(yystack.l_mark[-1].string, "%d", &input_delay) == 0)
		err(1, "delay specification %s could not be converted to "
		    "numeric at line %zu of file %s", yystack.l_mark[-1].string, line, cur_file);
	if (verbose) {
		fprintf(stderr, "Set input delay to %d ms\n", input_delay);
	}

	if (input_delay < DELAY_MIN)
		input_delay = DELAY_MIN;
	/*
	 * Fill in the timespec structure now ready for use later.
	 * The delay is specified in milliseconds so convert to timespec
	 * values
	 */
	delay_spec.tv_sec = input_delay / 1000;
	delay_spec.tv_nsec = (input_delay - 1000 * delay_spec.tv_sec) * 1000;
	if (verbose) {
		fprintf(stderr, "set delay to %jd.%jd\n",
		    (intmax_t)delay_spec.tv_sec,
		    (intmax_t)delay_spec.tv_nsec);
	}

	init_parse_variables(0);
 }
break;
case 38:
#line 381 "testlang_parse.y"
	{
	if (input_str != NULL) {
		warnx("%s, %zu: Discarding unused input string",
		    cur_file, line);
		free(input_str);
	}

	if ((input_str = malloc(strlen(yystack.l_mark[-1].string) + 1)) == NULL)
		err(2, "Cannot allocate memory for input string");

	strlcpy(input_str, yystack.l_mark[-1].string, strlen(yystack.l_mark[-1].string) + 1);
}
break;
case 39:
#line 396 "testlang_parse.y"
	{
	if (input_str != NULL) {
		warnx("%s, %zu: Discarding unused input string",
		    cur_file, line);
		free(input_str);
	}

	no_input = true;
 }
break;
case 41:
#line 408 "testlang_parse.y"
	{
	compare_streams(yystack.l_mark[-1].string, true);
}
break;
case 43:
#line 416 "testlang_parse.y"
	{
	compare_streams(yystack.l_mark[-1].string, false);
}
break;
case 47:
#line 427 "testlang_parse.y"
	{ assign_rets(data_number, yystack.l_mark[0].string); }
break;
case 48:
#line 428 "testlang_parse.y"
	{ assign_rets(data_number, yystack.l_mark[-1].string); }
break;
case 49:
#line 429 "testlang_parse.y"
	{ assign_rets(data_string, yystack.l_mark[0].string); }
break;
case 50:
#line 430 "testlang_parse.y"
	{ assign_rets(data_byte, (void *) yystack.l_mark[0].retval); }
break;
case 51:
#line 431 "testlang_parse.y"
	{ assign_rets(data_err, NULL); }
break;
case 52:
#line 432 "testlang_parse.y"
	{ assign_rets(data_ok, NULL); }
break;
case 53:
#line 433 "testlang_parse.y"
	{ assign_rets(data_null, NULL); }
break;
case 54:
#line 434 "testlang_parse.y"
	{ assign_rets(data_nonnull, NULL); }
break;
case 56:
#line 438 "testlang_parse.y"
	{
	assign_rets(data_var, yystack.l_mark[0].string);
 }
break;
case 57:
#line 443 "testlang_parse.y"
	{
	assign_rets(data_ref, yystack.l_mark[0].string);
 }
break;
case 58:
#line 447 "testlang_parse.y"
	{
	if (command.function != NULL)
		free(command.function);

	command.function = malloc(strlen(yystack.l_mark[0].string) + 1);
	if (command.function == NULL)
		err(1, "Could not allocate memory for function name");
	strcpy(command.function, yystack.l_mark[0].string);
 }
break;
case 59:
#line 458 "testlang_parse.y"
	{ yyval.vals = add_to_vals(data_number, yystack.l_mark[0].string); }
break;
case 60:
#line 460 "testlang_parse.y"
	{ yyval.vals = add_to_vals(data_number,
				get_numeric_var(yystack.l_mark[0].string)); }
break;
case 61:
#line 463 "testlang_parse.y"
	{
				yyval.vals = add_to_vals(data_byte, (void *) yystack.l_mark[0].retval);
			}
break;
case 62:
#line 467 "testlang_parse.y"
	{
				unsigned long i;
				unsigned long acount;

				acount = strtoul(yystack.l_mark[0].string, NULL, 10);
				for (i = 0; i < acount; i++) {
					yyval.vals = add_to_vals(data_number, yystack.l_mark[-2].string);
				}
			}
break;
case 63:
#line 477 "testlang_parse.y"
	{
				unsigned long i, acount;
				char *val;

				acount = strtoul(yystack.l_mark[0].string, NULL, 10);
				val = get_numeric_var(yystack.l_mark[-2].string);
				for (i = 0; i < acount; i++) {
					yyval.vals = add_to_vals(data_number, val);
				}
			}
break;
case 64:
#line 488 "testlang_parse.y"
	{
				unsigned long i, acount;

				acount = strtoul(yystack.l_mark[0].string, NULL, 10);
				for (i = 0; i < acount; i++) {
					yyval.vals = add_to_vals(data_byte, (void *) yystack.l_mark[-2].retval); 
				}
			}
break;
case 65:
#line 497 "testlang_parse.y"
	{
				unsigned long i, acount;

				acount = strtoul(yystack.l_mark[0].string, NULL, 10);
				for (i = 0; i < acount; i++) {
					yyval.vals = add_to_vals(data_string,
					    (void *) yystack.l_mark[-2].string); 
				}
			}
break;
case 68:
#line 511 "testlang_parse.y"
	{ yyval.string = get_numeric_var(yystack.l_mark[0].string); }
break;
case 69:
#line 513 "testlang_parse.y"
	{ yyval.string = numeric_or(yystack.l_mark[-2].string, yystack.l_mark[0].string); }
break;
case 71:
#line 517 "testlang_parse.y"
	{ assign_arg(data_static, yystack.l_mark[-1].string); }
break;
case 73:
#line 518 "testlang_parse.y"
	{ assign_arg(data_static, yystack.l_mark[0].string); }
break;
case 75:
#line 519 "testlang_parse.y"
	{ assign_arg(data_static, yystack.l_mark[0].string); }
break;
case 77:
#line 520 "testlang_parse.y"
	{ assign_arg(data_byte, yystack.l_mark[0].retval); }
break;
case 79:
#line 521 "testlang_parse.y"
	{ assign_arg(data_static, yystack.l_mark[0].string); }
break;
case 81:
#line 522 "testlang_parse.y"
	{ assign_arg(data_static, yystack.l_mark[0].string); }
break;
case 83:
#line 523 "testlang_parse.y"
	{ assign_arg(data_static, yystack.l_mark[0].string); }
break;
case 85:
#line 524 "testlang_parse.y"
	{ assign_arg(data_var, yystack.l_mark[0].string); }
break;
case 87:
#line 525 "testlang_parse.y"
	{ assign_arg(data_null, yystack.l_mark[0].string); }
break;
#line 3184 "testlang_parse.c"
    default:
        break;
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark -= yym;
#endif
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
        {
            fprintf(stderr, "%s[%d]: after reduction, ", YYDEBUGSTR, yydepth);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
            if (!yytrial)
#endif /* YYBTYACC */
                fprintf(stderr, "result is <%s>, ", YYSTYPE_TOSTRING(yystos[YYFINAL], yyval));
#endif
            fprintf(stderr, "shifting from state 0 to final state %d\n", YYFINAL);
        }
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        *++yystack.p_mark = yyloc;
#endif
        if (yychar < 0)
        {
#if YYBTYACC
            do {
            if (yylvp < yylve)
            {
                /* we're currently re-reading tokens */
                yylval = *yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylloc = *yylpp++;
#endif
                yychar = *yylexp++;
                break;
            }
            if (yyps->save)
            {
                /* in trial mode; save scanner results for future parse attempts */
                if (yylvp == yylvlim)
                {   /* Enlarge lexical value queue */
                    size_t p = (size_t) (yylvp - yylvals);
                    size_t s = (size_t) (yylvlim - yylvals);

                    s += YYLVQUEUEGROWTH;
                    if ((yylexemes = (YYINT *)realloc(yylexemes, s * sizeof(YYINT))) == NULL)
                        goto yyenomem;
                    if ((yylvals   = (YYSTYPE *)realloc(yylvals, s * sizeof(YYSTYPE))) == NULL)
                        goto yyenomem;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    if ((yylpsns   = (YYLTYPE *)realloc(yylpsns, s * sizeof(YYLTYPE))) == NULL)
                        goto yyenomem;
#endif
                    yylvp   = yylve = yylvals + p;
                    yylvlim = yylvals + s;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpp   = yylpe = yylpsns + p;
                    yylplim = yylpsns + s;
#endif
                    yylexp  = yylexemes + p;
                }
                *yylexp = (YYINT) YYLEX;
                *yylvp++ = yylval;
                yylve++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                *yylpp++ = yylloc;
                yylpe++;
#endif
                yychar = *yylexp++;
                break;
            }
            /* normal operation, no conflict encountered */
#endif /* YYBTYACC */
            yychar = YYLEX;
#if YYBTYACC
            } while (0);
#endif /* YYBTYACC */
            if (yychar < 0) yychar = YYEOF;
#if YYDEBUG
            if (yydebug)
            {
                if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
                fprintf(stderr, "%s[%d]: state %d, reading token %d (%s)\n",
                                YYDEBUGSTR, yydepth, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == YYEOF) goto yyaccept;
        goto yyloop;
    }
    if (((yyn = yygindex[yym]) != 0) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
    {
        fprintf(stderr, "%s[%d]: after reduction, ", YYDEBUGSTR, yydepth);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
            fprintf(stderr, "result is <%s>, ", YYSTYPE_TOSTRING(yystos[yystate], yyval));
#endif
        fprintf(stderr, "shifting from state %d to state %d\n", *yystack.s_mark, yystate);
    }
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    *++yystack.s_mark = (YYINT) yystate;
    *++yystack.l_mark = yyval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    *++yystack.p_mark = yyloc;
#endif
    goto yyloop;
#if YYBTYACC

    /* Reduction declares that this path is valid. Set yypath and do a full parse */
yyvalid:
    if (yypath) YYABORT;
    while (yyps->save)
    {
        YYParseState *save = yyps->save;
        yyps->save = save->save;
        save->save = yypath;
        yypath = save;
    }
#if YYDEBUG
    if (yydebug)
        fprintf(stderr, "%s[%d]: state %d, CONFLICT trial successful, backtracking to state %d, %d tokens\n",
                        YYDEBUGSTR, yydepth, yystate, yypath->state, (int)(yylvp - yylvals - yypath->lexeme));
#endif
    if (yyerrctx)
    {
        yyFreeState(yyerrctx);
        yyerrctx = NULL;
    }
    yylvp          = yylvals + yypath->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yylpp          = yylpsns + yypath->lexeme;
#endif
    yylexp         = yylexemes + yypath->lexeme;
    yychar         = YYEMPTY;
    yystack.s_mark = yystack.s_base + (yypath->yystack.s_mark - yypath->yystack.s_base);
    memcpy (yystack.s_base, yypath->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
    yystack.l_mark = yystack.l_base + (yypath->yystack.l_mark - yypath->yystack.l_base);
    memcpy (yystack.l_base, yypath->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark = yystack.p_base + (yypath->yystack.p_mark - yypath->yystack.p_base);
    memcpy (yystack.p_base, yypath->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
    yystate        = yypath->state;
    goto yyloop;
#endif /* YYBTYACC */

yyoverflow:
    YYERROR_CALL("yacc stack overflow");
#if YYBTYACC
    goto yyabort_nomem;
yyenomem:
    YYERROR_CALL("memory exhausted");
yyabort_nomem:
#endif /* YYBTYACC */
    yyresult = 2;
    goto yyreturn;

yyabort:
    yyresult = 1;
    goto yyreturn;

yyaccept:
#if YYBTYACC
    if (yyps->save) goto yyvalid;
#endif /* YYBTYACC */
    yyresult = 0;

yyreturn:
#if defined(YYDESTRUCT_CALL)
    if (yychar != YYEOF && yychar != YYEMPTY)
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        YYDESTRUCT_CALL("cleanup: discarding token", yychar, &yylval, &yylloc);
#else
        YYDESTRUCT_CALL("cleanup: discarding token", yychar, &yylval);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */

    {
        YYSTYPE *pv;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        YYLTYPE *pp;

        for (pv = yystack.l_base, pp = yystack.p_base; pv <= yystack.l_mark; ++pv, ++pp)
             YYDESTRUCT_CALL("cleanup: discarding state",
                             yystos[*(yystack.s_base + (pv - yystack.l_base))], pv, pp);
#else
        for (pv = yystack.l_base; pv <= yystack.l_mark; ++pv)
             YYDESTRUCT_CALL("cleanup: discarding state",
                             yystos[*(yystack.s_base + (pv - yystack.l_base))], pv);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
    }
#endif /* defined(YYDESTRUCT_CALL) */

#if YYBTYACC
    if (yyerrctx)
    {
        yyFreeState(yyerrctx);
        yyerrctx = NULL;
    }
    while (yyps)
    {
        YYParseState *save = yyps;
        yyps = save->save;
        save->save = NULL;
        yyFreeState(save);
    }
    while (yypath)
    {
        YYParseState *save = yypath;
        yypath = save->save;
        save->save = NULL;
        yyFreeState(save);
    }
#endif /* YYBTYACC */
    yyfreestack(&yystack);
    return (yyresult);
}
