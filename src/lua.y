
%union {
	
}

%glr-parser
%expect 1
%expect-rr 1 

%token T_NAME
%token T_NUMBER
%token T_STRING
%token T_SEMICOLON
%token T_RETURN
%token T_BREAK
%token T_ASSIGN
%token T_DO
%token T_END
%token T_REPEAT
%token T_UNTIL
%token T_IF
%token T_THEN
%token T_ELSE
%token T_ELSEIF
%token T_FOR
%token T_IN
%token T_COMMA
%token T_FUNCTION
%token T_LOCAL
%token T_DOT
%token T_COLON
%token T_OP 
%token T_CP
%token T_OB 
%token T_CB
%token T_OC 
%token T_CC
%token T_NIL
%token T_TRUE
%token T_FALSE
%token T_VARGS
%token T_NOT
%token T_LEN
%token T_EQ
%token T_WHILE
%token T_ADD
%token T_MINUS
%token T_MUL
%token T_DIV
%token T_POW
%token T_MOD
%token T_CONCAT
%token T_LESS
%token T_LE
%token T_GREATE
%token T_GE
%token T_NE
%token T_AND
%token T_OR

%{

#include "luam.h"

%}

%%

block: chunk {}

chunk: statlist laststatpart
	| statlist

laststatpart: laststat
	| laststat T_SEMICOLON

statlist: 
	| statlist statpart

statpart: stat
	| stat T_SEMICOLON

laststat: T_RETURN 
	| T_RETURN explist
	| T_BREAK

stat: varlist T_ASSIGN explist
	| functioncall %dprec 1
	| T_DO block T_END
	| T_REPEAT block T_UNTIL exp
	| T_IF exp T_THEN block elseiflist elsepart T_END
	| T_FOR T_NAME T_ASSIGN exp T_COMMA exp T_DO block T_END
	| T_FOR T_NAME T_ASSIGN exp T_COMMA exp T_COMMA exp T_DO block T_END
	| T_FOR namelist T_IN explist T_DO block T_END
	| T_FUNCTION funcname funcbody
	| T_LOCAL T_FUNCTION T_NAME funcbody
	| T_LOCAL namelist
	| T_LOCAL namelist T_ASSIGN explist
	| T_WHILE exp T_DO block T_END


elseiflist: 
	| elseiflist elseifpart

elseifpart: T_ELSEIF exp T_THEN block

elsepart: 
	| T_ELSE block

funcname: T_NAME funcnamememberlist funcnamelastmemberpart

funcnamememberlist: 
	| funcnamememberlist funcnamememberpart

funcnamememberpart: T_DOT T_NAME

funcnamelastmemberpart: 
	| T_COLON T_NAME

varlist: var
	| varlist T_COMMA var

var: T_NAME
	| prefixexp T_OB exp T_CB
	| prefixexp T_DOT T_NAME

namelist: T_NAME
	| namelist T_COMMA T_NAME

explist: exp
	| explist T_COMMA exp

exp: exp0
	| exp T_OR exp0

exp0: exp1
	| exp0 T_AND exp1

exp1: exp2
	| exp1 T_LESS exp2
	| exp1 T_LE exp2
	| exp1 T_GREATE exp2
	| exp1 T_GE exp2
	| exp1 T_EQ exp2
	| exp1 T_NE exp2

exp2: exp3
	| exp2 T_CONCAT exp3

exp3: exp4
	| exp3 T_ADD exp4
	| exp3 T_MINUS exp4

exp4: exp5
	| exp4 T_MUL exp5
	| exp4 T_DIV exp5
	| exp4 T_MOD exp5

exp5: exp6
	| T_NOT exp5
	| T_MINUS exp5
	| T_LEN exp5

exp6: expterm
	| exp6 T_POW expterm

expterm: T_NIL
	| T_FALSE
	| T_TRUE
	| T_NUMBER
	| T_STRING
	| T_VARGS
	| function
	| prefixexp
	| tableconstructor

prefixexp: var
	| functioncall %dprec 2
	| T_OP exp T_CP

functioncall: prefixexp args
	| prefixexp T_COLON T_NAME args

args: T_OP T_CP
	| T_OP explist T_CP
	| tableconstructor
	| T_STRING

function: T_FUNCTION funcbody

funcbody: T_OP T_CP block T_END
	| T_OP parlist T_CP block T_END

parlist: namelist
	| namelist T_COMMA T_VARGS
	| T_VARGS

tableconstructor: T_OC T_CC
	| T_OC fieldlist T_CC

fieldlistnotailfieldseq: field
	| fieldlistnotailfieldseq fieldsep field

fieldlist: fieldlistnotailfieldseq
	| fieldlistnotailfieldseq fieldsep

fieldsep: T_COMMA
	| T_SEMICOLON

field: T_OB exp T_CB T_ASSIGN exp
	| T_NAME T_ASSIGN exp
	| exp

fieldlist: T_COMMA
	| T_SEMICOLON

%%

