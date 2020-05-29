#ifndef _yy_defines_h_
#define _yy_defines_h_

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
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union {
	char *string;
	ct_data_t *retval;
	wchar_t	*vals;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
extern YYSTYPE yylval;

#endif /* _yy_defines_h_ */
