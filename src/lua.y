%token <sz> T_NAME 
%token <sz> T_NUMBER 
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
%token <lineno> T_FUNCTION
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
%token <sz> T_COMMENT
%token TM_DEFINE
%token TM_IFDEF
%token TM_ENDIF
%token TM_ELSE
%token TM_MACRO_DEV_START 
%token TM_MACRO_DEV_END

%{

#include "luam.h"
#include "ast.h"

extern block_t *root_block;

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
	struct exp0_t *exp0;
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
	struct macro_stat_t *macro_stat;
	struct macro_elsepart_t *macro_elsepart;
	struct functioncall_t *functioncall;
	struct args_t *args;
	double num;
	char *sz;
	int lineno;
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
%nterm <macro_stat> macro_stat
%nterm <macro_elsepart> macro_elsepart

%%

block: chunk { $$ = create_block($1); root_block = $$;}

chunk: statlist laststatpart { $$ = create_chunk($1, $2); }
	| statlist { $$ = create_chunk($1, 0); }

laststatpart: laststat { $$ = create_laststatpart($1, 0); }
	| laststat T_SEMICOLON { $$ = create_laststatpart($1, 1); }

statlist:                { $$ = create_statlist(0, 0);}
	| statlist statpart  { $$ = create_statlist($1, $2);}

statpart: stat           { $$ = create_statpart($1, 0, 0);}
	| stat T_SEMICOLON   { $$ = create_statpart($1, 1, 0);}
	| macro_stat { $$ = create_statpart(0, 0, $1);}

macro_stat: TM_DEFINE T_NAME 						{ $$ = create_macro_stat(macro_stat_define, $2, 0, 0);}
	| TM_IFDEF T_NAME block macro_elsepart TM_ENDIF	{ $$ = create_macro_stat(macro_stat_ifdef, $2, $3, $4);}
	| TM_MACRO_DEV_START block TM_MACRO_DEV_END		{ $$ = create_macro_stat(macro_stat_macro_dev_condition, 0, $2, 0);}

macro_elsepart: 	{ $$ = create_macro_elsepart(0);}
	| TM_ELSE block 	{ $$ = create_macro_elsepart($2);}

laststat: T_RETURN       { $$ = create_laststat(laststat_type_return, 0);}
	| T_RETURN explist   { $$ = create_laststat(laststat_type_return, $2);}
	| T_BREAK            { $$ = create_laststat(laststat_type_break, 0);}

prefixexp: var   			{ $$ = create_prefixexp($1, 0, 0);}
	| functioncall          { $$ = create_prefixexp(0, $1, 0);}
	| T_OP exp T_CP         { $$ = create_prefixexp(0, 0, $2);}
	
stat: varlist T_ASSIGN explist { $$ = create_stat(stat_type_varlist_assgin_explist, $1, $3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1);}
	| functioncall { $$ = create_stat(stat_type_functioncall, 0, 0, $1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1);}
	| T_DO block T_END { $$ = create_stat(stat_type_do_block_end, 0, 0, 0, $2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1);}
	| T_REPEAT block T_UNTIL exp { $$ = create_stat(stat_type_repeat_block_until_end, 0, 0, 0, $2, $4, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1);}
	| T_IF exp T_THEN block elseiflist elsepart T_END { $$ = create_stat(stat_type_if, 0, 0, 0, $4, $2, $5, $6, 0, 0, 0, 0, 0, 0, 0, -1);}
	| T_FOR T_NAME T_ASSIGN exp T_COMMA exp T_DO block T_END { $$ = create_stat(stat_type_for_loop, 0, 0, 0, $8, 0, 0, 0, $4, $6, 0, 0, 0, $2, 0, -1);}
	| T_FOR T_NAME T_ASSIGN exp T_COMMA exp T_COMMA exp T_DO block T_END { $$ = create_stat(stat_type_for_loop_step, 0, 0, 0, $10, 0, 0, 0, $4, $6, $8, 0, 0, $2, 0, -1);}
	| T_FOR namelist T_IN explist T_DO block T_END { $$ = create_stat(stat_type_for_in, 0, $4, 0, $6, 0, 0, 0, 0, 0, 0, 0, 0, 0, $2, -1);}
	| T_FUNCTION funcname funcbody { $$ = create_stat(stat_type_function, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, $2, $3, 0, 0, $1);}
	| T_LOCAL T_FUNCTION T_NAME funcbody { $$ = create_stat(stat_type_local_function, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, $4, $3, 0, $2);}
	| T_LOCAL namelist { $$ = create_stat(stat_type_local_namelist, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, $2, -1);}
	| T_LOCAL namelist T_ASSIGN explist { $$ = create_stat(stat_type_local_namelist_assgin_explist, 0, $4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, $2, -1);}
	| T_WHILE exp T_DO block T_END { $$ = create_stat(stat_type_while, 0, 0, 0, $4, $2, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1);}

elseiflist:  { $$ = create_elseiflist(0, 0);}
	| elseiflist elseifpart { $$ = create_elseiflist($1, $2);}

elseifpart: T_ELSEIF exp T_THEN block { $$ = create_elseifpart($2, $4);}

elsepart:  { $$ = create_elsepart(0);}
	| T_ELSE block { $$ = create_elsepart($2);}

funcname: T_NAME funcnamememberlist funcnamelastmemberpart { $$ = create_funcname($1, $2, $3);}

funcnamememberlist:  { $$ = create_funcnamememberlist(0, 0);}
	| funcnamememberlist funcnamememberpart   { $$ = create_funcnamememberlist($1, $2);}

funcnamememberpart: T_DOT T_NAME   { $$ = create_funcnamememberpart($2);}

funcnamelastmemberpart:    { $$ = create_funcnamelastmemberpart(0);}
	| T_COLON T_NAME   { $$ = create_funcnamelastmemberpart($2);}

varlist: var  { $$ = create_varlist($1, 0);}
	| varlist T_COMMA var  { $$ = create_varlist($3, $1);}

var: T_NAME                       { $$ = create_var($1, 0, 0);}
	| prefixexp T_OB exp T_CB     { $$ = create_var(0, $3, $1);}
	| prefixexp T_DOT T_NAME      { $$ = create_var($3, 0, $1);}

namelist: T_NAME                { $$ = create_namelist($1, 0);}
	| namelist T_COMMA T_NAME   { $$ = create_namelist($3, $1);}

explist: exp                    { $$ = create_explist( $1, 0);}
	| explist T_COMMA exp    	{ $$ = create_explist( $3, $1);}

exp: exp0                        	{ $$ = create_exp($1, 0);}
	| exp T_OR exp0            		{ $$ = create_exp($3, $1);}

exp0: exp1               	{ $$ = create_exp0($1, 0);}
	| exp0 T_AND exp1    	{ $$ = create_exp0($3, $1);}

exp1: exp2               	{ $$ = create_exp1($1, 0, 0);}
	| exp1 T_LESS exp2    	{ $$ = create_exp1($3, $1, T_LESS);}
	| exp1 T_LE exp2      	{ $$ = create_exp1($3, $1, T_LE);}
	| exp1 T_GREATE exp2   	{ $$ = create_exp1($3, $1, T_GREATE);}
	| exp1 T_GE exp2 		{ $$ = create_exp1($3, $1, T_GE);}
	| exp1 T_EQ exp2 		{ $$ = create_exp1($3, $1, T_EQ);}
	| exp1 T_NE exp2 		{ $$ = create_exp1($3, $1, T_NE);}

exp2: exp3 					{ $$ = create_exp2($1, 0);}
	| exp2 T_CONCAT exp3 	{ $$ = create_exp2($3, $1);}

exp3: exp4 					{ $$ = create_exp3($1, 0, 0);}
	| exp3 T_ADD exp4       { $$ = create_exp3($3, $1, T_ADD);}
	| exp3 T_MINUS exp4  	{ $$ = create_exp3($3, $1, T_MINUS);}

exp4: exp5 					{ $$ = create_exp4($1, 0, 0);}
	| exp4 T_MUL exp5 		{ $$ = create_exp4($3, $1, T_MUL);}
	| exp4 T_DIV exp5 		{ $$ = create_exp4($3, $1, T_DIV);}
	| exp4 T_MOD exp5 		{ $$ = create_exp4($3, $1, T_MOD);}

exp5: exp6 					{ $$ = create_exp5($1, 0, 0);}
	| T_NOT exp5 			{ $$ = create_exp5(0, $2, T_NOT);}
	| T_MINUS exp5  		{ $$ = create_exp5(0, $2, T_MINUS);}
	| T_LEN exp5 			{ $$ = create_exp5(0, $2, T_LEN);}

exp6: expterm   			{ $$ = create_exp6($1, 0);}
	| exp6 T_POW expterm    { $$ = create_exp6($3, $1);}

expterm: T_NIL 				{ $$ = create_expterm(expterm_type_nil, 0, 0, 0, 0);}
	| T_FALSE				{ $$ = create_expterm(expterm_type_false, 0, 0, 0, 0);}
	| T_TRUE 				{ $$ = create_expterm(expterm_type_true, 0, 0, 0, 0);}
	| T_NUMBER     			{ $$ = create_expterm(expterm_type_number, $1, 0, 0, 0);}
	| T_STRING    			{ $$ = create_expterm(expterm_type_string, $1, 0, 0, 0);}
	| T_VARGS               { $$ = create_expterm(expterm_type_vargs, 0, 0, 0, 0);}
	| function 				{ $$ = create_expterm(expterm_type_function, 0, $1, 0, 0);}
	| prefixexp             { $$ = create_expterm(expterm_type_prefixexp, 0, 0, $1, 0);}
	| tableconstructor      { $$ = create_expterm(expterm_type_tableconstructor, 0, 0, 0, $1);}

functioncall: prefixexp args  		{ $$ = create_functioncall($1, $2, 0);}
	| prefixexp T_COLON T_NAME args { $$ = create_functioncall($1, $4, $3);}

args: T_OP T_CP						{ $$ = create_args(0, 0, 0);}
	| T_OP explist T_CP				{ $$ = create_args($2, 0, 0);}
	| tableconstructor 				{ $$ = create_args(0, $1, 0);}
	| T_STRING 						{ $$ = create_args(0, 0, $1);}

function: T_FUNCTION funcbody  			{ $$ = create_function($2, $1);}

funcbody: T_OP T_CP block T_END  		{ $$ = create_funcbody($3, 0);}
	| T_OP parlist T_CP block T_END 	{ $$ = create_funcbody($4, $2);}

parlist: namelist  					{ $$ = create_parlist($1, 0);}
	| namelist T_COMMA T_VARGS 		{ $$ = create_parlist($1, 1);}
	| T_VARGS 						{ $$ = create_parlist(0, 1);}

tableconstructor: T_OC T_CC 		{ $$ = create_tableconstructor(0);}
	| T_OC fieldlist T_CC 			{ $$ = create_tableconstructor($2);}

fieldlistnotailfieldseq: field  				{ $$ = create_fieldlistnotailfieldseq($1, 0, 0);}
	| fieldlistnotailfieldseq fieldsep field    { $$ = create_fieldlistnotailfieldseq($3, $1, $2);}

fieldlist: fieldlistnotailfieldseq  	{ $$ = create_fieldlist($1, 0);}
	| fieldlistnotailfieldseq fieldsep 	{ $$ = create_fieldlist($1, $2);}

fieldsep: T_COMMA 		{ $$ = create_fieldsep(fieldsep_type_comma);}
	| T_SEMICOLON 		{ $$ = create_fieldsep(fieldsep_type_semicolon);}

field: T_OB exp T_CB T_ASSIGN exp  	{ $$ = create_field($5, $2, 0);}
	| T_NAME T_ASSIGN exp  			{ $$ = create_field($3, 0, $1);}
	| exp 							{ $$ = create_field($1, 0, 0);}

%%

