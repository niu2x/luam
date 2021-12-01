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


macro_stat_t* create_macro_stat(int type, name_t *name, block_t *block, macro_elsepart_t *elsepart) {
	macro_stat_t *macro_stat = ALLOC(macro_stat_t) ;
	macro_stat->type = type;
	macro_stat->name = name;
	macro_stat->block = block;
	macro_stat->macro_elsepart = elsepart;
	return macro_stat;
}

macro_elsepart_t* create_macro_elsepart(block_t *block) {
	macro_elsepart_t *macro_elsepart = ALLOC(macro_elsepart_t) ;
	macro_elsepart->block = block;
	return macro_elsepart;
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

statpart_t* create_statpart(stat_t *stat, int semicolon, macro_stat_t *macro_stat) {
	statpart_t *result = ALLOC(statpart_t) ;
	result->stat = stat ;
	result->semicolon = semicolon ;
	result->macro_stat = macro_stat;
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
	, namelist_t *namelist
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
	stat->namelist = namelist ;
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

expterm_t* create_expterm(int type, char *sz, function_t *function, prefixexp_t *prefixexp, tableconstructor_t *tableconstructor) {
	expterm_t *expterm = ALLOC(expterm_t) ;
	expterm->type = type ;
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


DEF_DESTROY(block) {
	if(!block) return;
	destroy_chunk(block->chunk);
	FREE(block);
}

DEF_DESTROY(args) {
	if(!args) return;
	destroy_tableconstructor(args->tableconstructor);
	destroy_explist(args->explist);
	FREE(args->sz);
	FREE(args);
}

DEF_DESTROY(macro_elsepart) {
	if(!macro_elsepart) return;
	destroy_block(macro_elsepart->block);
	FREE(macro_elsepart);
}

DEF_DESTROY(macro_stat) {
	if(!macro_stat) return;
	destroy_macro_elsepart(macro_stat->macro_elsepart);
	destroy_block(macro_stat->block);
	FREE(macro_stat->name);
	FREE(macro_stat);
}

DEF_DESTROY(chunk) {
	if(!chunk) return;
	destroy_statlist(chunk->statlist);
	destroy_laststatpart(chunk->laststatpart);
	FREE(chunk);
}

DEF_DESTROY(elseiflist) {
	if(!elseiflist) return;
	destroy_elseifpart(elseiflist->elseifpart);
	destroy_elseiflist(elseiflist->elseiflist);
	FREE(elseiflist);
}

DEF_DESTROY(elseifpart) {
	if(!elseifpart) return;
	destroy_exp(elseifpart->exp);
	destroy_block(elseifpart->block);
	FREE(elseifpart);
}

DEF_DESTROY(elsepart) {
	if(!elsepart) return;
	destroy_block(elsepart->block);
	FREE(elsepart);
}

DEF_DESTROY(exp) {
	if(!exp) return;
	destroy_exp(exp->left);
	destroy_exp0(exp->right);
	FREE(exp);
}

DEF_DESTROY(exp0) {
	if(!exp0) return;
	destroy_exp0(exp0->left);
	destroy_exp1(exp0->right);
	FREE(exp0);
}

DEF_DESTROY(exp1) {
	if(!exp1) return;
	destroy_exp1(exp1->left);
	destroy_exp2(exp1->right);
	FREE(exp1);
}

DEF_DESTROY(exp2) {
	if(!exp2) return;
	destroy_exp2(exp2->left);
	destroy_exp3(exp2->right);
	FREE(exp2);
}	

DEF_DESTROY(exp3) {
	if(!exp3) return;
	destroy_exp3(exp3->left);
	destroy_exp4(exp3->right);
	FREE(exp3);
}

DEF_DESTROY(exp4) {
	if(!exp4) return;
	destroy_exp4(exp4->left);
	destroy_exp5(exp4->right);
	FREE(exp4);
}

DEF_DESTROY(exp5) {
	if(!exp5) return;
	destroy_exp5(exp5->exp5);
	destroy_exp6(exp5->exp6);
	FREE(exp5);
}

DEF_DESTROY(exp6) {
	if(!exp6) return;
	destroy_exp6(exp6->left);
	destroy_expterm(exp6->right);
	FREE(exp6);
}

DEF_DESTROY(explist) {
	if(!explist) return;
	destroy_explist(explist->explist);
	destroy_exp(explist->exp);
	FREE(explist);
}

DEF_DESTROY(expterm) {
	if(!expterm) return;
	FREE(expterm->sz);

	destroy_function(expterm->function);
	destroy_prefixexp(expterm->prefixexp);
	destroy_tableconstructor(expterm->tableconstructor);

	FREE(expterm);
}

DEF_DESTROY(field) {
	if(!field) return;
	FREE(field->name);
	destroy_exp(field->name_exp);
	destroy_exp(field->exp);

	FREE(field);
}

DEF_DESTROY(fieldlist) {
	if(!fieldlist) return;
	destroy_fieldsep(fieldlist->fieldsep);
	destroy_fieldlistnotailfieldseq(fieldlist->fieldlistnotailfieldseq);
	FREE(fieldlist);
}

DEF_DESTROY(fieldlistnotailfieldseq) {
	if(!fieldlistnotailfieldseq) return;
	destroy_fieldlistnotailfieldseq(fieldlistnotailfieldseq->fieldlistnotailfieldseq);
	destroy_field(fieldlistnotailfieldseq->field);
	destroy_fieldsep(fieldlistnotailfieldseq->fieldsep);
	FREE(fieldlistnotailfieldseq);
}

DEF_DESTROY(fieldsep) {
	if(!fieldsep) return;
	FREE(fieldsep);
}

DEF_DESTROY(funcbody) {
	if(!funcbody) return;
	destroy_parlist(funcbody->parlist);
	destroy_block(funcbody->block);
	FREE(funcbody);
}

DEF_DESTROY(funcname) {
	if(!funcname) return;
	FREE(funcname->name);
	destroy_funcnamememberlist (funcname->funcnamememberlist);
	destroy_funcnamelastmemberpart (funcname->funcnamelastmemberpart);

	FREE(funcname);
}

DEF_DESTROY(funcnamelastmemberpart) {
	if(!funcnamelastmemberpart) return;
	FREE(funcnamelastmemberpart->name);
	FREE(funcnamelastmemberpart);
}

DEF_DESTROY(funcnamememberlist) {
	if(!funcnamememberlist) return;

	destroy_funcnamememberlist(funcnamememberlist->funcnamememberlist);
	destroy_funcnamememberpart(funcnamememberlist->funcnamememberpart);

	FREE(funcnamememberlist);
}

DEF_DESTROY(funcnamememberpart) {
	if(!funcnamememberpart) return;
	FREE(funcnamememberpart->name);
	FREE(funcnamememberpart);
}

DEF_DESTROY(function) {
	if(!function) return;
	destroy_funcbody(function->funcbody);
	FREE(function);
}

DEF_DESTROY(functioncall) {
	if(!functioncall) return;
	destroy_prefixexp(functioncall->prefixexp);
	destroy_args(functioncall->args);
	FREE(functioncall->name);
	FREE(functioncall);
}

DEF_DESTROY(laststat) {
	if(!laststat) return;
	destroy_explist(laststat->explist);
	FREE(laststat);
}

DEF_DESTROY(laststatpart) {
	if(!laststatpart) return;
	destroy_laststat(laststatpart->laststat);
	FREE(laststatpart);
}

DEF_DESTROY(namelist) {
	if(!namelist) return;
	FREE(namelist->name);
	destroy_namelist(namelist->namelist);
	FREE(namelist);
}

DEF_DESTROY(parlist) {
	if(!parlist) return;
	destroy_namelist(parlist->namelist);
	FREE(parlist);
}

DEF_DESTROY(prefixexp) {
	if(!prefixexp) return;
	destroy_var(prefixexp->var);
	destroy_functioncall(prefixexp->functioncall);
	destroy_exp(prefixexp->exp);
	FREE(prefixexp);
}

DEF_DESTROY(stat) {
	if(!stat) return;
	destroy_varlist(stat->varlist);
	destroy_explist(stat->explist);
	destroy_functioncall(stat->functioncall);
	destroy_block(stat->block);
	destroy_exp(stat->exp);
	destroy_elseiflist(stat->elseiflist);
	destroy_elsepart(stat->elsepart);
	destroy_exp(stat->exp0);
	destroy_exp(stat->exp1);
	destroy_exp(stat->exp2);
	destroy_funcname(stat->funcname);
	destroy_funcbody(stat->funcbody);
	FREE(stat->name);
	destroy_namelist(stat->namelist);
	FREE(stat);
}

DEF_DESTROY(statlist) {
	if(!statlist) return;
	destroy_statlist(statlist->statlist);
	destroy_statpart(statlist->statpart);
	FREE(statlist);
}

DEF_DESTROY(statpart) {
	if(!statpart) return;
	destroy_stat(statpart->stat);
	destroy_macro_stat(statpart->macro_stat);
	FREE(statpart);
}

DEF_DESTROY(tableconstructor) {
	if(!tableconstructor) return;
	destroy_fieldlist(tableconstructor->fieldlist);
	FREE(tableconstructor);
}

DEF_DESTROY(var) {
	if(!var) return;
	FREE(var->name);
	destroy_prefixexp(var->prefixexp);
	destroy_exp(var->exp);
	FREE(var);
}

DEF_DESTROY(varlist) {
	if(!varlist) return;
	destroy_varlist(varlist->varlist);
	destroy_var(varlist->var);
	FREE(varlist);
}
