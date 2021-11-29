#include "ast.h"

#include <stdlib.h>

#define ALLOC(type) (type*)malloc(sizeof(type))
#define FREE(ptr) free(ptr)

block_t* create_block(chunk_t *chunk) {
	block_t *block = ALLOC(block_t) ;
	block->chunk = chunk ;
	return block ;
}

chunk_t* create_chunk(statlist_t *statlist, laststatpart_t *laststatpart) {
	chunk_t *chunk = ALLOC(chunk_t) ;
	chunk->statlist = statlist ;
	chunk->laststatpart = laststatpart ;
	return chunk ;
}

laststatpart_t* create_laststatpart(laststat_t *laststat, int semicolon) {
	laststatpart_t *laststatpart = ALLOC(laststatpart_t) ;
	laststatpart->laststat = laststat ;
	laststatpart->semicolon = semicolon ;
	return laststatpart ;
}

statlist_t* create_statlist(statlist_t *statlist, statpart_t *statpart) {

	statlist_t *result = ALLOC(statlist_t) ;
	result->statlist = statlist ;
	result->statpart = statpart ;
	return result ;
}

statpart_t* create_statpart(stat_t *stat, int semicolon) {
	statpart_t *result = ALLOC(statpart_t) ;
	result->stat = stat ;
	result->semicolon = semicolon ;
	return result ;
}

laststat_t* create_laststat(int type, explist_t *explist) {
	laststat_t *laststat = ALLOC(laststat_t) ;
	laststat->type = type ;
	laststat->explist = explist ;
	return laststat ;
}

prefixexp_t* create_prefixexp(var_t *var, functioncall_t *functioncall, exp_t *exp) {
	prefixexp_t *prefixexp = ALLOC(prefixexp_t) ;
	prefixexp->var = var ;
	prefixexp->functioncall = functioncall ;
	prefixexp->exp = exp ;
	return prefixexp ;
}

stat_t *create_stat(int type, 
	varlist_t * varlist
	, explist_t *explist
	, functioncall_t *functioncall
	, block_t *block
	, exp_t *exp
	, elseiflist_t *elseiflist
	, elsepart_t *elsepart
	, exp_t * exp0
	, exp_t * exp1
	, exp_t * exp2
	, funcname_t *funcname
	, funcbody_t *funcbody
	, name_t *name
) {
	stat_t *stat = ALLOC(stat_t) ;
	stat->type = type ;
	stat->varlist = varlist ;
	stat->explist = explist ;
	stat->functioncall = functioncall ;
	stat->block = block ;
	stat->exp = exp ;
	stat->elseiflist = elseiflist ;
	stat->elsepart = elsepart ;
	stat->exp0 = exp0 ;
	stat->exp1 = exp1 ;
	stat->exp2 = exp2 ;
	stat->funcname = funcname ;
	stat->funcbody = funcbody ;
	stat->name = name ;
	return stat ;
}

elseiflist_t *create_elseiflist(elseiflist_t *elseiflist, elseifpart_t *elseifpart) {
	elseiflist_t *result = ALLOC(elseiflist_t) ;
	result->elseifpart = elseifpart ;
	result->elseiflist = elseiflist ;
	return result ;
}

elseifpart_t* create_elseifpart(exp_t *exp, block_t *block) {
	elseifpart_t *elseifpart = ALLOC(elseifpart_t) ;
	elseifpart->block = block ;
	elseifpart->exp = exp ;
	return elseifpart ;
}

elsepart_t* create_elsepart(block_t *block) {
	elsepart_t *elsepart = ALLOC(elsepart_t) ;
	elsepart->block = block ;
	return elsepart ;
}

funcname_t *create_funcname(name_t *name, funcnamememberlist_t *funcnamememberlist, funcnamelastmemberpart_t *funcnamelastmemberpart) {

	funcname_t *funcname = ALLOC(funcname_t) ;
	funcname->name = name ;
	funcname->funcnamememberlist = funcnamememberlist ;
	funcname->funcnamelastmemberpart = funcnamelastmemberpart ;
	return funcname ;
}

funcnamememberlist_t *create_funcnamememberlist(funcnamememberlist_t *funcnamememberlist, funcnamememberpart_t *funcnamememberpart) {
	funcnamememberlist_t *result = ALLOC(funcnamememberlist_t) ;
	result->funcnamememberlist = funcnamememberlist ;
	result->funcnamememberpart = funcnamememberpart ;
	return result ;
}

funcnamememberpart_t *create_funcnamememberpart(name_t *name) {
	funcnamememberpart_t *funcnamememberpart = ALLOC(funcnamememberpart_t) ;
	funcnamememberpart->name = name ;
	return funcnamememberpart ;
}

funcnamelastmemberpart_t *create_funcnamelastmemberpart(name_t *name) {
	funcnamelastmemberpart_t *funcnamelastmemberpart = ALLOC(funcnamelastmemberpart_t) ;
	funcnamelastmemberpart->name = name ;
	return funcnamelastmemberpart ;
}

varlist_t *create_varlist(var_t *var, varlist_t *varlist) {
	varlist_t *result = ALLOC(varlist_t) ;
	result->var = var ;
	result->varlist = varlist ;
	return result ;
}

var_t *create_var(name_t *name, exp_t *exp, prefixexp_t *prefixexp) {
	var_t *var = ALLOC(var_t) ;
	var->name = name ;
	var->exp = exp ;
	var->prefixexp = prefixexp ;
	return var ;
}

namelist_t *create_namelist(name_t *name, namelist_t *namelist) {
	namelist_t *result = ALLOC(namelist_t) ;
	result->name = name ;
	result->namelist = namelist ;
	return result ;
}

explist_t* create_explist(exp_t *exp, explist_t *explist) {
	explist_t *result = ALLOC(explist_t) ;
	result->exp = exp ;
	result->explist = explist ;
	return result ;
}

exp_t * create_exp(exp0_t *exp0, exp_t *exp) {
	exp_t *result = ALLOC(exp_t) ;
	result->right = exp0 ;
	result->left = exp ;
	return result ;
}

exp0_t* create_exp0(exp1_t *exp1, exp0_t *exp0) {
	exp0_t *result = ALLOC(exp0_t) ;
	result->left = exp0 ;
	result->right = exp1 ;
	return result ;
}

exp1_t* create_exp1(exp2_t *exp2, exp1_t *exp1, int type) {
	exp1_t *result = ALLOC(exp1_t) ;
	result->left = exp1 ;
	result->right = exp2 ;
	result->type = type ;
	return result ;
}

exp2_t* create_exp2(exp3_t *exp3, exp2_t *exp2) {
	exp2_t *result = ALLOC(exp2_t) ;
	result->left = exp2 ;
	result->right = exp3 ;
	return result ;
}

exp3_t* create_exp3(exp4_t *exp4, exp3_t *exp3, int type) {
	exp3_t *result = ALLOC(exp3_t) ;
	result->left = exp3 ;
	result->right = exp4 ;
	result->type = type ;
	return result ;
}
exp4_t* create_exp4(exp5_t *exp5, exp4_t *exp4, int type) {
	exp4_t *result = ALLOC(exp4_t) ;
	result->left = exp4 ;
	result->right = exp5 ;
	result->type = type ;
	return result ;
}
exp5_t* create_exp5(exp6_t *exp6, exp5_t *exp5, int type) {
	exp5_t *result = ALLOC(exp5_t) ;
	result->exp6 = exp6 ;
	result->exp5 = exp5 ;
	result->type = type ;
	return result ;
}
exp6_t* create_exp6(expterm_t *expterm, exp6_t *exp6) {
	exp6_t *result = ALLOC(exp6_t) ;
	result->left = exp6 ;
	result->right = expterm ;
	return result ;
}

expterm_t* create_expterm(int type, double num, char *sz, function_t *function, prefixexp_t *prefixexp, tableconstructor_t *tableconstructor) {
	expterm_t *expterm = ALLOC(expterm_t) ;
	expterm->type = type ;
	expterm->num = num ;
	expterm->sz = sz ;
	expterm->function = function ;
	expterm->prefixexp = prefixexp ;
	expterm->tableconstructor = tableconstructor ;
	return expterm ;
}

functioncall_t *create_functioncall(prefixexp_t *prefixexp, args_t *args, name_t *name) {
	functioncall_t *functioncall = ALLOC(functioncall_t) ;
	functioncall->prefixexp = prefixexp ;
	functioncall->args = args ;
	functioncall->name = name ;
	return functioncall ;
}

args_t *create_args(explist_t *explist, tableconstructor_t *tableconstructor, char *sz) {
	args_t *args = ALLOC(args_t) ;
	args->tableconstructor = tableconstructor ;
	args->explist = explist ;
	args->sz = sz ;
	return args ;
}

function_t* create_function(funcbody_t *funcbody) {
	function_t *function = ALLOC(function_t) ;
	function->funcbody = funcbody ;
	return function ;
}

funcbody_t* create_funcbody(block_t *block, parlist_t *parlist) {

	funcbody_t *funcbody = ALLOC(funcbody_t) ;
	funcbody->parlist = parlist ;
	funcbody->block = block ;
	return funcbody ;

}

parlist_t *create_parlist(namelist_t *namelist, int vargs) {
	parlist_t *parlist = ALLOC(parlist_t) ;
	parlist->namelist = namelist ;
	parlist->vargs = vargs ;
	return parlist ;
}

tableconstructor_t *create_tableconstructor(fieldlist_t *fieldlist) {
	tableconstructor_t *tableconstructor = ALLOC(tableconstructor_t) ;
	tableconstructor->fieldlist = fieldlist ;
	return tableconstructor ;
}

fieldlist_t* create_fieldlist(fieldlistnotailfieldseq_t *fieldlistnotailfieldseq, fieldsep_t *fieldsep) {
	fieldlist_t *fieldlist = ALLOC(fieldlist_t) ;
	fieldlist->fieldsep = fieldsep ;
	fieldlist->fieldlistnotailfieldseq = fieldlistnotailfieldseq ;
	return fieldlist ;
}

fieldlistnotailfieldseq_t* create_fieldlistnotailfieldseq(field_t *field, fieldlistnotailfieldseq_t *fieldlistnotailfieldseq, fieldsep_t *fieldsep) {
	fieldlistnotailfieldseq_t *result = ALLOC(fieldlistnotailfieldseq_t) ;
	result->field = field ;
	result->fieldlistnotailfieldseq = fieldlistnotailfieldseq ;
	result->fieldsep = fieldsep ;
	return result ;
}

fieldsep_t *create_fieldsep(int type) {
	fieldsep_t *fieldsep = ALLOC(fieldsep_t) ;
	fieldsep->type = type ;
	return fieldsep ;
}

field_t* create_field(exp_t *exp, exp_t *name_exp, name_t *name) {
	field_t *field = ALLOC(field_t) ;
	field->exp = exp ;
	field->name_exp = name_exp ;
	field->name = name ;
	return field ;
}
