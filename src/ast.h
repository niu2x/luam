#ifndef LUAM_AST_H
#define LUAM_AST_H

#define AST_NAME_FIELD(name, _type) struct _type##_t * name;
#define AST_FIELD(_type) AST_NAME_FIELD(_type, _type)

enum {
	laststat_type_return,
	laststat_type_break,

	fieldsep_type_comma,
	fieldsep_type_semicolon,

	expterm_type_nil,
	expterm_type_false,
	expterm_type_true,
	expterm_type_number,
	expterm_type_string,
	expterm_type_vargs,
	expterm_type_function,
	expterm_type_prefixexp,
	expterm_type_tableconstructor,

	stat_type_varlist_assgin_explist,
	stat_type_functioncall,
	stat_type_do_block_end,
	stat_type_repeat_block_until_end,
	stat_type_if,
	stat_type_for_loop,
	stat_type_for_loop_step,
	stat_type_for_in,
	stat_type_function,
	stat_type_local_function,
	stat_type_local_namelist,
	stat_type_local_namelist_assgin_explist,
	stat_type_while,

	macro_stat_define,
	macro_stat_ifdef,
	macro_stat_macro_dev_condition,

	// prefixexp_type_var,
	// prefixexp_type_functioncall,
	// prefixexp_type_op_exp_cp,
};

typedef char name_t; 

struct prefixexp_t;
struct block_t;
struct namelist_t;
struct exp_t;
struct fieldlistnotailfieldseq_t;

typedef struct field_t {
	name_t *name;
	AST_FIELD(exp)
	AST_NAME_FIELD(name_exp, exp)
} field_t;

typedef struct fieldlistnotailfieldseq_t {
	AST_FIELD(field)
	AST_FIELD(fieldlistnotailfieldseq)
	AST_FIELD(fieldsep)
} fieldlistnotailfieldseq_t;

typedef struct fieldsep_t {
	int type;
} fieldsep_t;

typedef struct fieldlist_t {
	AST_FIELD(fieldlistnotailfieldseq)
	AST_FIELD(fieldsep)
}fieldlist_t;

typedef struct tableconstructor_t {
	AST_FIELD(fieldlist)
}tableconstructor_t;

typedef struct args_t {
	AST_FIELD(explist);
	AST_FIELD(tableconstructor);
	char *sz;
} args_t;

typedef struct functioncall_t {
	AST_FIELD(args);
	AST_FIELD(prefixexp);
	name_t *name;
} functioncall_t;

typedef struct parlist_t {
	int vargs;
	AST_FIELD(namelist);
} parlist_t;

typedef struct funcbody_t {
	AST_FIELD(block);
	AST_FIELD(parlist);
} funcbody_t;

typedef struct function_t {
	AST_FIELD(funcbody)
	int lineno;
}function_t;


typedef struct expterm_t {
	int type;

	struct {
		AST_FIELD(function)
		AST_FIELD(prefixexp)
		AST_FIELD(tableconstructor)
		char *sz;
	};

}expterm_t;

typedef struct exp6_t {
	struct exp6_t *left;
	AST_NAME_FIELD(right, expterm);
} exp6_t;

typedef struct exp5_t {
	struct exp5_t *exp5;
	AST_FIELD(exp6);
	int type;
} exp5_t;

typedef struct exp4_t {
	struct exp4_t *left;
	AST_NAME_FIELD(right, exp5);
	int type;
} exp4_t;

typedef struct exp3_t {
	struct exp3_t *left;
	AST_NAME_FIELD(right, exp4);
	int type;
} exp3_t;

typedef struct exp2_t {
	struct exp2_t *left;
	AST_NAME_FIELD(right, exp3);
} exp2_t;

typedef struct exp1_t {
	struct exp1_t *left;
	AST_NAME_FIELD(right, exp2);
	int type;
} exp1_t;

typedef struct exp0_t {
	struct exp0_t *left;
	AST_NAME_FIELD(right, exp1);
} exp0_t;

typedef struct exp_t {
	struct exp_t *left;
	AST_NAME_FIELD(right, exp0);
} exp_t;

typedef struct explist_t {
	struct explist_t *explist;
	AST_FIELD(exp);
} explist_t;

typedef struct namelist_t {
	struct namelist_t *namelist;
	name_t *name;
} namelist_t;

typedef struct var_t {
	name_t *name;
	AST_FIELD(exp);
	AST_FIELD(prefixexp);
} var_t;

typedef struct varlist_t {
	AST_FIELD(var);
	struct varlist_t *varlist;
} varlist_t;

typedef struct funcnamelastmemberpart_t {
	name_t *name;
} funcnamelastmemberpart_t;

typedef struct funcnamememberpart_t {
	name_t *name;
} funcnamememberpart_t;

typedef struct funcnamememberlist_t {
	struct funcnamememberlist_t *funcnamememberlist;
	AST_FIELD(funcnamememberpart);
} funcnamememberlist_t;

typedef struct funcname_t {
	name_t *name;
	AST_FIELD(funcnamememberlist)
	AST_FIELD(funcnamelastmemberpart)
} funcname_t;

typedef struct elsepart_t {
	AST_FIELD(block);
} elsepart_t;

typedef struct elseifpart_t {
	AST_FIELD(exp)
	AST_FIELD(block)
} elseifpart_t;

typedef struct elseiflist_t {
	struct elseiflist_t *elseiflist;
	AST_FIELD(elseifpart)
} elseiflist_t;

typedef struct stat_t {
	int type;

	struct {
		AST_FIELD(varlist)
		AST_FIELD(explist)
		AST_FIELD(namelist)
		AST_FIELD(functioncall)
		AST_FIELD(exp);
		AST_FIELD(elseiflist);
		AST_FIELD(elsepart);
		AST_NAME_FIELD(exp0, exp);
		AST_NAME_FIELD(exp1, exp);
		AST_NAME_FIELD(exp2, exp);
		AST_FIELD(block);
		AST_FIELD(funcname)
		AST_FIELD(funcbody);
		name_t *name;
		int lineno;
	};
}stat_t;

typedef struct prefixexp_t {
	int type;

	struct {
		AST_FIELD(var)
		AST_FIELD(functioncall)
		AST_FIELD(exp)
	};

} prefixexp_t;


typedef struct laststat_t {
	int type;
	struct {
		AST_FIELD(explist)
	};
} laststat_t;

typedef struct macro_elsepart_t {
	AST_FIELD(block);
}macro_elsepart_t;

typedef struct macro_stat_t {
	int type;
	name_t *name;
	AST_FIELD(macro_elsepart);
	AST_FIELD(block);
} macro_stat_t;

typedef struct statpart_t {
	AST_FIELD(macro_stat);
	AST_FIELD(stat)
	int semicolon;
}statpart_t;

typedef struct statlist_t {
	AST_FIELD(statlist)
	AST_FIELD(statpart)
}statlist_t;

typedef struct laststatpart_t {
	AST_FIELD(laststat)
	int semicolon;
} laststatpart_t;

typedef struct chunk_t {
	AST_FIELD(statlist)
	AST_FIELD(laststatpart)
} chunk_t;

typedef struct block_t{
	AST_FIELD(chunk)
} block_t;




block_t* create_block(chunk_t *);

chunk_t* create_chunk(statlist_t *, laststatpart_t *);

laststatpart_t* create_laststatpart(laststat_t *, int semicolon);

statlist_t* create_statlist(statlist_t *, statpart_t *);

statpart_t* create_statpart(stat_t *, int semicolon, macro_stat_t *);

macro_stat_t* create_macro_stat(int type, name_t *, block_t *, macro_elsepart_t *);

macro_elsepart_t* create_macro_elsepart(block_t *);

laststat_t* create_laststat(int type, explist_t *);

prefixexp_t* create_prefixexp(var_t *, functioncall_t *, exp_t *);

stat_t *create_stat(int type, 
	varlist_t *
	, explist_t *
	, functioncall_t *
	, block_t *
	, exp_t *

	, elseiflist_t *
	, elsepart_t *

	, exp_t * exp0
	, exp_t * exp1
	, exp_t * exp2

	, funcname_t *
	, funcbody_t *
	
	, name_t *
	, namelist_t *
	, int lineno
);

elseiflist_t *create_elseiflist(elseiflist_t *, elseifpart_t *);

elseifpart_t* create_elseifpart(exp_t *, block_t *);

elsepart_t* create_elsepart(block_t *);

funcname_t *create_funcname(name_t *, funcnamememberlist_t *, funcnamelastmemberpart_t *);

funcnamememberlist_t *create_funcnamememberlist(funcnamememberlist_t*, funcnamememberpart_t *);

funcnamememberpart_t *create_funcnamememberpart(name_t *);

funcnamelastmemberpart_t *create_funcnamelastmemberpart(name_t *);

varlist_t *create_varlist(var_t *, varlist_t *);

var_t *create_var(name_t *, exp_t *, prefixexp_t *);

namelist_t *create_namelist(name_t *, namelist_t *);

explist_t* create_explist(exp_t *, explist_t*);

exp_t * create_exp(exp0_t *, exp_t *);

exp0_t* create_exp0(exp1_t *, exp0_t *);

exp1_t* create_exp1(exp2_t *, exp1_t *, int type);

exp2_t* create_exp2(exp3_t *, exp2_t *);

exp3_t* create_exp3(exp4_t *, exp3_t *, int type);
exp4_t* create_exp4(exp5_t *, exp4_t *, int type);
exp5_t* create_exp5(exp6_t *, exp5_t *, int type);
exp6_t* create_exp6(expterm_t *, exp6_t *);

expterm_t* create_expterm(int type,  char *sz, function_t *, prefixexp_t *, tableconstructor_t *);

functioncall_t *create_functioncall(prefixexp_t *, args_t *, name_t *);

args_t *create_args(explist_t *, tableconstructor_t *, char *sz);

function_t* create_function(funcbody_t *, int lineno);

funcbody_t* create_funcbody(block_t*, parlist_t*);

parlist_t *create_parlist(namelist_t *, int vargs);

tableconstructor_t *create_tableconstructor(fieldlist_t *);

fieldlist_t* create_fieldlist(fieldlistnotailfieldseq_t *, fieldsep_t *);

fieldlistnotailfieldseq_t* create_fieldlistnotailfieldseq(field_t *, fieldlistnotailfieldseq_t*, fieldsep_t *);

fieldsep_t *create_fieldsep(int);

field_t* create_field(exp_t*, exp_t *name_exp, name_t *);


#define DEF_DESTROY(type) void destroy_##type(type##_t *type)


DEF_DESTROY(args);
DEF_DESTROY(block);
DEF_DESTROY(chunk);
DEF_DESTROY(elseiflist);
DEF_DESTROY(elseifpart);
DEF_DESTROY(elsepart);
DEF_DESTROY(exp);
DEF_DESTROY(macro_stat);
DEF_DESTROY(macro_elsepart);
DEF_DESTROY(exp0);
DEF_DESTROY(exp1);
DEF_DESTROY(exp2);
DEF_DESTROY(exp3);
DEF_DESTROY(exp4);
DEF_DESTROY(exp5);
DEF_DESTROY(exp6);
DEF_DESTROY(explist);
DEF_DESTROY(expterm);
DEF_DESTROY(field);
DEF_DESTROY(fieldlist);
DEF_DESTROY(fieldlistnotailfieldseq);
DEF_DESTROY(fieldsep);
DEF_DESTROY(funcbody);
DEF_DESTROY(funcname);
DEF_DESTROY(funcnamelastmemberpart);
DEF_DESTROY(funcnamememberlist);
DEF_DESTROY(funcnamememberpart);
DEF_DESTROY(function);
DEF_DESTROY(functioncall);
DEF_DESTROY(laststat);
DEF_DESTROY(laststatpart);
DEF_DESTROY(name);
DEF_DESTROY(namelist);
DEF_DESTROY(parlist);
DEF_DESTROY(prefixexp);
DEF_DESTROY(stat);
DEF_DESTROY(statlist);
DEF_DESTROY(statpart);
DEF_DESTROY(tableconstructor);
DEF_DESTROY(var);
DEF_DESTROY(varlist);

block_t *luam_parse(char *);

#endif