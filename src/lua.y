%token <sz> T_NAME 
%token <num> T_NUMBER 
%token <sz> T_STRING 
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
#include "ast.h"

%}



%union {
	struct field_t *field;
	struct fieldlistnotailfieldseq_t *fieldlistnotailfieldseq;
	struct fieldsep_t *fieldsep;
	struct fieldlist_t *fieldlist;
	struct tableconstructor_t *tableconstructor;
	struct parlist_t *parlist;
	struct funcbody_t *funcbody;
	struct function_t *function;
	struct expterm_t *expterm;
	struct exp6_t *exp6;
	struct exp5_t *exp5;
	struct exp4_t *exp4;
	struct exp3_t *exp3;
	struct exp2_t *exp2;
	struct exp1_t *exp1;
	struct exp_t *exp;
	struct explist_t *explist;
	struct namelist_t *namelist;
	struct var_t *var;
	struct varlist_t *varlist;
	struct funcnamelastmemberpart_t *funcnamelastmemberpart;
	struct funcnamememberpart_t *funcnamememberpart;
	struct funcnamememberlist_t *funcnamememberlist;
	struct funcname_t *funcname;
	struct elsepart_t *elsepart;
	struct elseifpart_t *elseifpart;
	struct elseiflist_t *elseiflist;
	struct stat_t *stat;
	struct prefixexp_t *prefixexp;
	struct laststat_t *laststat;
	struct statpart_t *statpart;
	struct statlist_t *statlist;
	struct laststatpart_t *laststatpart;
	struct chunk_t *chunk;
	struct block_t *block;
	struct functioncall_t *functioncall;
	struct args_t *args;
	double num;
	char *sz;
}

%nterm <block> block
%nterm <chunk> chunk
%nterm <laststatpart> laststatpart
%nterm <statlist> statlist
%nterm <statpart> statpart
%nterm <laststat> laststat
%nterm <prefixexp> prefixexp
%nterm <stat> stat
%nterm <elseiflist> elseiflist
%nterm <elseifpart> elseifpart
%nterm <elsepart> elsepart
%nterm <funcname> funcname
%nterm <funcnamememberlist> funcnamememberlist
%nterm <funcnamememberpart> funcnamememberpart
%nterm <funcnamelastmemberpart> funcnamelastmemberpart
%nterm <varlist> varlist
%nterm <var> var
%nterm <namelist> namelist
%nterm <explist> explist
%nterm <exp> exp
%nterm <exp0> exp0
%nterm <exp1> exp1
%nterm <exp2> exp2
%nterm <exp3> exp3
%nterm <exp4> exp4
%nterm <exp5> exp5
%nterm <exp6> exp6
%nterm <expterm> expterm
%nterm <functioncall> functioncall
%nterm <args> args
%nterm <function> function
%nterm <funcbody> funcbody
%nterm <parlist> parlist
%nterm <tableconstructor> tableconstructor
%nterm <fieldlistnotailfieldseq> fieldlistnotailfieldseq
%nterm <fieldlist> fieldlist
%nterm <fieldsep> fieldsep
%nterm <field> field

%%

block: chunk { $$ = create_block($1); }

chunk: statlist laststatpart { $$ = create_chunk($1, $2); }
	| statlist { $$ = create_chunk($1, 0); }

laststatpart: laststat { $$ = create_laststatpart($1, 0); }
	| laststat T_SEMICOLON { $$ = create_laststatpart($1, 1); }

statlist:                { $$ = create_statlist(0, 0);}
	| statlist statpart  { $$ = create_statlist($1, $2);}

statpart: stat           { $$ = create_statpart($1, 0);}
	| stat T_SEMICOLON   { $$ = create_statpart($1, 1);}

laststat: T_RETURN       { $$ = create_laststat(laststat_type_return, 0);}
	| T_RETURN explist   { $$ = create_laststat(laststat_type_return, $2);}
	| T_BREAK            { $$ = create_laststat(laststat_type_break, 0);}

prefixexp: var   			{ $$ = create_prefixexp($1, 0, 0);}
	| functioncall          { $$ = create_prefixexp(0, $1, 0);}
	| T_OP exp T_CP         { $$ = create_prefixexp(0, 0, $2);}
	
stat: varlist T_ASSIGN explist { $$ = create_stat(stat_type_varlist_assgin_explist, $1, $3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);}
	| functioncall { $$ = create_stat(stat_type_functioncall, 0, 0, $1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);}
	| T_DO block T_END { $$ = create_stat(stat_type_do_block_end, 0, 0, 0, $1, 0, 0, 0, 0, 0, 0, 0, 0, 0);}
	| T_REPEAT block T_UNTIL exp { $$ = create_stat(stat_type_repeat_block_until_end, 0, 0, 0, $2, $4, 0, 0, 0, 0, 0, 0, 0, 0);}
	| T_IF exp T_THEN block elseiflist elsepart T_END { $$ = create_stat(stat_type_if, 0, 0, 0, $4, $2, $5, $6, 0, 0, 0, 0, 0, 0);}
	| T_FOR T_NAME T_ASSIGN exp T_COMMA exp T_DO block T_END { $$ = create_stat(stat_type_for_loop, 0, 0, 0, 0, 0, 0, 0, $4, $6, 0, 0, 0, $2);}
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

%%

