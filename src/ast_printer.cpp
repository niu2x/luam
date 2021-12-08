#define LUAM_HOOK_LINE
#define LUAM_HOOK_FUNCTION

#include "ast_printer.h"

#include <string.h>
#include <stack>

extern "C" {
#include "lua.tab.h"
}

#include <map>
#include <string>

struct symbol_t {
	std::string name;
};

struct ci_func_t {
	uint16_t line;
};

static std::stack<ci_func_t> ci_stack;

static std::map<std::string, symbol_t> symtab;

#define PRINT(type) static void print_ast(const type##_t* self, std::ostream &os)
#define PRINT_SUB(name) if(self->name) {print_ast(self->name, os);}

PRINT(args);
PRINT(elseiflist);
PRINT(elseifpart);
PRINT(elsepart);
PRINT(exp);
PRINT(exp0);
PRINT(exp1);
PRINT(exp2);
PRINT(exp3);
PRINT(exp4);
PRINT(exp5);
PRINT(exp6);
PRINT(explist);
PRINT(expterm);
PRINT(field);
PRINT(fieldlist);
PRINT(fieldlistnotailfieldseq);
PRINT(fieldsep);
PRINT(funcname);
PRINT(funcnamelastmemberpart);
PRINT(funcnamememberlist);
PRINT(funcnamememberpart);
PRINT(function);
PRINT(functioncall);
PRINT(laststat);
PRINT(laststatpart);
PRINT(name);
PRINT(namelist);
PRINT(parlist);
PRINT(macro_stat);
PRINT(macro_elsepart);
PRINT(prefixexp);
PRINT(stat);
PRINT(statlist);
PRINT(statpart);
PRINT(tableconstructor);
PRINT(var);

static void print_ast(const block_t* self, std::ostream &os, bool is_funcbody=false, int lineno=-1);
static void print_ast(const chunk_t* self, std::ostream &os, bool is_funcbody=false, int lineno=-1);
static void print_ast(const funcbody_t* self, std::ostream &os, int lineno=-1);

PRINT(varlist);

#define output(k) \
	os << k;

static int level = 0;

#define newline() {						\
	output("\n"); 						\
	for(int i = 0; i < level; i ++){	\
		output("\t");					\
	}									\
}

#define nonewline_push() { \
	++ level;			\
}

#define newline_pop() { \
	-- level;			\
	newline();			\
}

static char module_name[1024];

void print_ast_root(const block_t *block, std::ostream &os, const char *p_module_name) {
	strncpy(module_name, p_module_name, 1024);
	symtab.clear();
	ci_stack = std::stack<ci_func_t>();
	level = 0;
	os << "local ____MYG = _G;\n";
	print_ast(block, os);
	symtab.clear();
}

static void print_ast(const block_t* self, std::ostream &os, bool is_funcbody, int lineno){
	if(self->chunk) {print_ast(self->chunk, os, is_funcbody, lineno);}
}

PRINT(args) {
	if(self->sz) {
		output(" ");
		output(self->sz);
	}
	else if(self->tableconstructor){
		output(" ");
		PRINT_SUB(tableconstructor);
	}
	else{
		output("(");
		PRINT_SUB(explist);
		output(")");
	}
}

static void print_ast(const chunk_t* self, std::ostream &os, bool is_funcbody, int lineno){
	#if defined(LUAM_HOOK_FUNCTION)
	if(is_funcbody){
		newline();
		output("____MYG.enter_lua_function('");
		output(module_name);
		output("', ");
		output(lineno);
		output(");");
		newline();
	}
	ci_stack.push({.line = lineno});
	#endif




	PRINT_SUB(statlist);

	#if defined(LUAM_HOOK_FUNCTION)
	ci_stack.pop();
	if(is_funcbody){
		newline();
		output("____MYG.exit_lua_function();");
		newline();
	}
	#endif

	PRINT_SUB(laststatpart);
	
}

PRINT(elseiflist) {
	PRINT_SUB(elseiflist);
	PRINT_SUB(elseifpart);
}

PRINT(elseifpart) {
	newline_pop();
	output("elseif ");
	PRINT_SUB(exp);
	output(" then")
	nonewline_push();
	PRINT_SUB(block);
}

PRINT(elsepart) {
	if(self->block){
		newline_pop();
		output("else");
		nonewline_push();
		PRINT_SUB(block);
	}
}

PRINT(exp) {
	if(self->left) {
		PRINT_SUB(left);
		output(" or ");
	}
	PRINT_SUB(right);
}

PRINT(exp0) {
	if(self->left) {
		PRINT_SUB(left);
		output(" and ");
	}
	PRINT_SUB(right);
}

PRINT(exp1) {
	if(self->left) {
		PRINT_SUB(left);
		switch(self->type) {
			case T_LESS:
				output(" < ");
				break;
			case T_LE:
				output(" <= ");
				break;
			case T_GREATE:
				output(" > ");
				break;
			case T_GE:
				output(" >= ");
				break;
			case T_EQ:
				output(" == ");
				break;
			case T_NE:
				output(" ~= ");
				break;
		}
	}
	PRINT_SUB(right);
}

PRINT(exp2) {
	if(self->left) {
		PRINT_SUB(left);
		output(" .. ");
	}
	PRINT_SUB(right);
}	

PRINT(exp3) {
	if(self->left) {
		PRINT_SUB(left);
		switch(self->type) {
			case T_ADD:
				output("+");
				break;
			case T_MINUS:
				output("-");
				break;
		}
	}
	PRINT_SUB(right);
}

PRINT(exp4) {
	if(self->left) {
		PRINT_SUB(left);
		switch(self->type) {
			case T_MUL:
				output("*");
				break;
			case T_DIV:
				output("/");
				break;
			case T_MOD:
				output("%");
				break;
		}
	}
	PRINT_SUB(right);
}

PRINT(exp5) {
	if(self->exp6){
		PRINT_SUB(exp6);
	}
	else{
		switch(self->type) {
			case T_MINUS:
				output("-");
				break;
			case T_NOT:
				output("not ");
				break;
			case T_LEN:
				output("# ");
				break;
		}
		PRINT_SUB(exp5);
	}
}

PRINT(exp6) {
	if(self->left) {
		PRINT_SUB(left);
		output("^")
	}
	PRINT_SUB(right);
}

PRINT(explist) {
	if(self->explist) {
		PRINT_SUB(explist);
		output(" , ");
	}
	PRINT_SUB(exp);
}

PRINT(expterm) {
	switch(self->type) {
		case expterm_type_nil:
			output("nil");
			break;
		case expterm_type_false:
			output("false");
			break;
		case expterm_type_true:
			output("true");
			break;
		case expterm_type_number:
			output(self->sz);
			break;
		case expterm_type_string:
			output(self->sz);
			break;
		case expterm_type_vargs:
			output("...");
			break;
		case expterm_type_function:
			PRINT_SUB(function);
			break;
		case expterm_type_prefixexp:
			PRINT_SUB(prefixexp);
			break;
		case expterm_type_tableconstructor:
			PRINT_SUB(tableconstructor);
			break;
	}
}

PRINT(field) {
	newline();
	if(self->name_exp) {
		output("[");
		PRINT_SUB(name_exp);
		output("]");
		output(" = ");
	}

	else if(self->name) {
		output(self->name);
		output(" = ");
	}

	PRINT_SUB(exp);
}

PRINT(fieldlist) {
	PRINT_SUB(fieldlistnotailfieldseq);
	PRINT_SUB(fieldsep);
}

PRINT(fieldlistnotailfieldseq) {
	PRINT_SUB(fieldlistnotailfieldseq);
	PRINT_SUB(fieldsep);
	PRINT_SUB(field);
}

PRINT(fieldsep) {
	switch(self->type){
		case fieldsep_type_comma: {
			output(",");
			break;
		}
		case fieldsep_type_semicolon: {
			output(";");
			break;
		}
	}
}

static void print_ast(const funcbody_t* self, std::ostream &os, int lineno) {
	output("(");
	PRINT_SUB(parlist);
	output(")");
	nonewline_push();

	if(self->block) {print_ast(self->block, os, true, lineno);}

	newline_pop();
	output("end");
}

PRINT(funcname) {
	output(self->name);
	PRINT_SUB(funcnamememberlist);
	PRINT_SUB(funcnamelastmemberpart);
}

PRINT(funcnamelastmemberpart) {
	if(self->name){
		output(":");
		output(self->name);
	}
}

PRINT(funcnamememberlist) {
	PRINT_SUB(funcnamememberlist);
	PRINT_SUB(funcnamememberpart);
}

PRINT(funcnamememberpart) {
	output(".");
	output(self->name);
}

PRINT(function) {
	output("function");
	print_ast(self->funcbody, os, self->lineno);
}

PRINT(functioncall) {
	PRINT_SUB(prefixexp);
	if(self->name){
		output(":");
		output(self->name);
	}
	PRINT_SUB(args);
}

PRINT(laststat) {
	switch(self->type) {
		case laststat_type_return:
			if(!ci_stack.empty()){
				newline();
				output("____MYG.exit_lua_function();");
				newline();
			}

			output("return ");
			PRINT_SUB(explist);
			break;
		case laststat_type_break:
			output("break ");
			break;
	}
}

PRINT(laststatpart) {
	newline();
	PRINT_SUB(laststat);
	if(self->semicolon)
		output(";");
}

PRINT(namelist) {
	if(self->namelist) {
		PRINT_SUB(namelist);
		output(",");
	}
	output(self->name);
}

PRINT(parlist) {
	if(self->namelist && self->vargs) {
		PRINT_SUB(namelist);
		output(", ...");
	}
	else if(self->namelist) {
		PRINT_SUB(namelist);
	}
	else {
		output("...");
	}
}

PRINT(prefixexp) {
	if(self->var) {
		PRINT_SUB(var);
	}
	else if(self->functioncall){
		PRINT_SUB(functioncall);
	}
	else{
		output("(");
		PRINT_SUB(exp);
		output(")");
	}
}

PRINT(stat) {
	switch(self->type){
		case stat_type_varlist_assgin_explist:
			PRINT_SUB(varlist);
			output(" = ");
			PRINT_SUB(explist);
			break;
		case stat_type_functioncall:
			PRINT_SUB(functioncall);
			break;
		case stat_type_do_block_end:
			output("do");
			nonewline_push();
			PRINT_SUB(block);
			newline_pop();
			output("end");
			break;
		case stat_type_repeat_block_until_end:
			output("repeat");
			nonewline_push();
			PRINT_SUB(block);
			newline_pop();
			output("until ");
			PRINT_SUB(exp);
			break;
		case stat_type_if:
			output("if ");
			PRINT_SUB(exp);
			output(" then");
			nonewline_push();
			PRINT_SUB(block);
			PRINT_SUB(elseiflist);
			PRINT_SUB(elsepart);
			newline_pop();
			output("end");
			break;
		case stat_type_for_loop:
			output("for ");
			output(self->name);
			output(" = ");
			PRINT_SUB(exp0);
			output(" , ");
			PRINT_SUB(exp1);
			output(" do");
			nonewline_push();
			PRINT_SUB(block);
			newline_pop();
			output("end");
			break;
		case stat_type_for_loop_step:
			output("for ");
			output(self->name);
			output(" = ");
			PRINT_SUB(exp0);
			output(" , ");
			PRINT_SUB(exp1);
			output(" , ");
			PRINT_SUB(exp2);
			output(" do");
			nonewline_push();
			PRINT_SUB(block);
			newline_pop();
			output("end");
			break;
		case stat_type_for_in:
			output("for ");
			PRINT_SUB(namelist);
			output(" in ");
			PRINT_SUB(explist);
			output(" do");
			nonewline_push();
			PRINT_SUB(block);
			newline_pop();
			output("end");
			break;
		case stat_type_function:
			output("function ");
			PRINT_SUB(funcname);
			print_ast(self->funcbody, os, self->lineno);
			break;
		case stat_type_local_function:
			output("local function ");
			output(self->name);
			print_ast(self->funcbody, os, self->lineno);
			break;
		case stat_type_local_namelist:
			output("local ");
			PRINT_SUB(namelist);
			break;
		case stat_type_local_namelist_assgin_explist:
			output("local ");
			PRINT_SUB(namelist);
			output(" = ");
			PRINT_SUB(explist);
			break;
		case stat_type_while:
			output("while ");
			PRINT_SUB(exp);
			output(" do");
			nonewline_push();
			PRINT_SUB(block);
			newline_pop();
			output("end");
			break;
	}
}

PRINT(statlist) {
	PRINT_SUB(statlist);
	PRINT_SUB(statpart);
}

PRINT(statpart) {
	newline();
	if(self->stat){
		PRINT_SUB(stat);
		if(self->semicolon)
			output(";");
		newline();
		#if defined(LUAM_HOOK_LINE)
		output("____MYG.lua_line = ____MYG.lua_line + 1;");
		#endif
		newline();
	}
	else{
		PRINT_SUB(macro_stat);
	}
}

PRINT(macro_stat) {
	switch(self->type){
		case macro_stat_define: {
			std::string name = self->name;
			symtab[name] = {
				 name,
			};
			break;
		}
		case macro_stat_ifdef: {
			std::string name = self->name;
			if(symtab.find(name) != symtab.end()){
				PRINT_SUB(block);
			}
			else{
				PRINT_SUB(macro_elsepart);
			}
			break;
		}
		case macro_stat_macro_dev_condition: {
			std::string name = "DEV";
			if(symtab.find(name) != symtab.end()){
				PRINT_SUB(block);
			}
		}
	}
}

PRINT(macro_elsepart) {
	PRINT_SUB(block);
}

PRINT(tableconstructor) {
	output("{");
	nonewline_push();
	PRINT_SUB(fieldlist);
	newline_pop();
	output("}");
}

PRINT(var) {
	if(self->prefixexp) {
		PRINT_SUB(prefixexp);
		if(self->exp) {
			output("[");
			PRINT_SUB(exp);
			output("]");
		}
		else{
			output(".");
			output(self->name);
		}
	}
	else{
		output(self->name);
	}
}

PRINT(varlist) {
	if(self->varlist){
		PRINT_SUB(varlist);
		output(", ");
	}
	PRINT_SUB(var);
}
