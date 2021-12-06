extern "C" {
#include "ast.h"
}


#include <niu2x/pipe.h>
#include <sstream>
#include <string>
#include <variant>

#include "json.hpp"

extern "C" {
#include "lua.tab.h"
}

using json = nlohmann::json;

#define PRINT(type)                                                            \
    static json print_ast(const type##_t* self, std::ostream& os)

template<class T>
json print_sub(const T *self_name, std::ostream &os) {
    if(self_name){
        return print_ast(self_name, os);
    }
    return "";
}

#define PRINT_SUB(name) print_sub(self->name, os)
#define JSON_SUB(name) j[#name] = print_sub(self->name, os)

PRINT(args);
PRINT(chunk);
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
PRINT(funcbody);
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
PRINT(prefixexp);
PRINT(stat);
PRINT(statlist);
PRINT(statpart);
PRINT(tableconstructor);
PRINT(var);
PRINT(block);
PRINT(varlist);

#define output(k) os << k;

static int level = 0;

#define newline()                                                              \
    {                                                                          \
        output("\n");                                                          \
        for (int i = 0; i < level; i++) {                                      \
            output("\t");                                                      \
        }                                                                      \
    }

#define nonewline_push()                                                       \
    {                                                                          \
        ++level;                                                               \
    }

#define newline_pop()                                                          \
    {                                                                          \
        --level;                                                               \
    }

json print_ast_root(const block_t* block, std::ostream& os)
{
    level = 0;
    return print_ast(block, os);
}

PRINT(block) { 
    return {
        {"type", "block"},
        {"chunk", PRINT_SUB(chunk)},
    }; 
}

PRINT(args)
{
    if (self->sz) {
        return {
            {"type", "args"},
            {"sz", self->sz},
        };
    } else if (self->tableconstructor) {
        return {
            {"type", "args"},
            {"tableconstructor", PRINT_SUB(tableconstructor)},
        };
    } else {
        return {
            {"type", "args"},
            {"explist", PRINT_SUB(explist)},
        };
    }
}

PRINT(chunk)
{
    PRINT_SUB(statlist);
    PRINT_SUB(laststatpart);

    return {
        {"type", "chunk"},
        {"statlist", PRINT_SUB(statlist)},
        {"laststatpart", PRINT_SUB(laststatpart)},
    };
}

PRINT(elseiflist)
{
    return {
        {"type", "elseiflist"},
        {"elseiflist", PRINT_SUB(elseiflist)},
        {"elseifpart", PRINT_SUB(elseifpart)},
    };
}

PRINT(elseifpart)
{

    return {
        {"type", "elseifpart"},
        {"exp", PRINT_SUB(exp)},
        {"block", PRINT_SUB(block)},
    };
}

PRINT(elsepart)
{
    if (self->block) {
        return {
            {"type", "elsepart"},
            {"block", PRINT_SUB(block)},
        };
    }
    return "";
}

PRINT(exp)
{
    json j = {{"right", PRINT_SUB(right)}, {"type", "exp"}};
    if (self->left) {
        j["left"] = PRINT_SUB(left);
    }
    return j;
}

PRINT(exp0)
{
    json j = {{"right", PRINT_SUB(right)}, {"type", "exp0"}};
    if (self->left) {
        j["left"] = PRINT_SUB(left);
    }
    return j;
}

PRINT(exp1)
{
    json j = {{"right", PRINT_SUB(right)}, {"type", "exp1"}};
    if (self->left) {
        j["left"] = PRINT_SUB(left);
        switch (self->type) {
            case T_LESS:
                j["subType"] = "<";
                break;
            case T_LE:
                j["subType"] = "<=";
                break;
            case T_GREATE:
                j["subType"] = ">";
                break;
            case T_GE:
                j["subType"] = ">=";
                break;
            case T_EQ:
                j["subType"] = "==";
                break;
            case T_NE:
                j["subType"] = "~=";
                break;
        }
    }
    return j;
}

PRINT(exp2)
{
    json j = {{"right", PRINT_SUB(right)}, {"type", "exp2"}};
    if (self->left) {
        j["left"] = PRINT_SUB(left);
    }
    return j;
}

PRINT(exp3)
{
    json j = {{"right", PRINT_SUB(right)}, {"type", "exp3"}};
    if (self->left) {
        j["left"] = PRINT_SUB(left);
        switch (self->type) {
            case T_ADD:
                j["subType"] = "+";
                break;
            case T_MINUS:
                j["subType"] = "-";
                break;
        }
    }
    return j;
}

PRINT(exp4)
{
    json j = {{"right", PRINT_SUB(right)}, {"type", "exp4"}};
    if (self->left) {
        j["left"] = PRINT_SUB(left);
        PRINT_SUB(left);
        switch (self->type) {
            case T_MUL:
                j["subType"] = "*";
                break;
            case T_DIV:
                j["subType"] = "/";
                break;
            case T_MOD:
                j["subType"] = "%";
                break;
        }
    }
    return j;
}

PRINT(exp5)
{
    json j = {{"type", "exp5"}};
    if (self->exp6) {
        j["exp6"] = PRINT_SUB(exp6);
    } else {
        switch (self->type) {
            case T_MINUS:
                j["subType"] = "-";
                break;
            case T_NOT:
                j["subType"] = "not";
                break;
            case T_LEN:
                j["subType"] = "#";
                break;
        }
        j["exp5"] = PRINT_SUB(exp5);
    }
    return j;
}

PRINT(exp6)
{
    json j = {{"right", PRINT_SUB(right)}, {"type", "exp6"}};
    if (self->left) {
        j["left"] = PRINT_SUB(left);
    }
    return j;
}

PRINT(explist)
{
    json j = {{"type", "explist"}};
    if (self->explist) {
        j["explist"] = PRINT_SUB(explist);
    }
    j["exp"] = PRINT_SUB(exp);
    return j;
}

#define DJ(_type) json j = {{"type", #_type}};

PRINT(expterm)
{
    DJ(expterm);
    switch (self->type) {
        case expterm_type_nil:
            j["subType"] = "nil";
            break;
        case expterm_type_false:
            j["subType"] = "false";
            break;
        case expterm_type_true:
            j["subType"] = "true";
            break;
        case expterm_type_number:
            j["subType"] = "number";
            j["sz"] = self->sz;
            break;
        case expterm_type_string:
            j["subType"] = "string";
            j["sz"] = self->sz;
            break;
        case expterm_type_vargs:
            j["subType"] = "vargs";
            break;
        case expterm_type_function:
            j["subType"] = "function";
            JSON_SUB(function);
            break;
        case expterm_type_prefixexp:
            j["subType"] = "prefixexp";
            JSON_SUB(prefixexp);
            break;
        case expterm_type_tableconstructor:
            j["subType"] = "tableconstructor";
            JSON_SUB(tableconstructor);
            break;
    }
    return j;
}

PRINT(field)
{
    DJ(field);
    if (self->name_exp) {
        JSON_SUB(name_exp);
    }

    else if (self->name) {
        j["name"] = self->name;
    }

    JSON_SUB(exp);
    return j;
}

PRINT(fieldlist)
{
    DJ(fieldlist);

    JSON_SUB(fieldlistnotailfieldseq);
    JSON_SUB(fieldsep);
    return j;
}

PRINT(fieldlistnotailfieldseq)
{
    DJ(fieldlistnotailfieldseq);
    JSON_SUB(fieldlistnotailfieldseq);
    JSON_SUB(fieldsep);
    JSON_SUB(field);
    return j;
}

PRINT(fieldsep)
{
    DJ(fieldsep);
    switch (self->type) {
        case fieldsep_type_comma: {
            j["subType"] = "comma";
            break;
        }
        case fieldsep_type_semicolon: {
            j["subType"] = "semicolon";
            break;
        }
    }
    return j;
}

PRINT(funcbody)
{
    DJ(funcbody);
    JSON_SUB(parlist);
    JSON_SUB(block);
    return j;
}

PRINT(funcname)
{
    DJ(funcname);
    j["name"] = self->name;
    JSON_SUB(funcnamememberlist);
    JSON_SUB(funcnamelastmemberpart);
    return j;
}

PRINT(funcnamelastmemberpart)
{
    DJ(funcnamelastmemberpart);
    if (self->name) {
        j["name"] = self->name;
    }
    return j;
}

PRINT(funcnamememberlist)
{
    DJ(funcnamememberlist);
    JSON_SUB(funcnamememberlist);
    JSON_SUB(funcnamememberpart);
    return j;
}

PRINT(funcnamememberpart)
{
    DJ(funcnamememberpart);
    j["name"] = self->name;
    return j;
}

PRINT(function)
{
    DJ(function);
    JSON_SUB(funcbody);
    return j;
}

PRINT(functioncall)
{
    DJ(functioncall);
    JSON_SUB(prefixexp);
    if (self->name) {
        j["name"] = self->name;
    }
    JSON_SUB(args);
    return j;
}

PRINT(laststat)
{
    DJ(laststat);
    switch (self->type) {
        case laststat_type_return:
            j["subType"] = "return";
            JSON_SUB(explist);
            break;
        case laststat_type_break:
            j["subType"] = "break";
            break;
    }
    return j;
}

PRINT(laststatpart)
{
    DJ(laststatpart);
    JSON_SUB(laststat);
    j["semicolon"] = self->semicolon == 1;

    return j;
}

PRINT(namelist)
{
    DJ(namelist);
    if (self->namelist) {
        JSON_SUB(namelist);
    }
    j["name"] = self->name;
    return j;
}

PRINT(parlist)
{
    DJ(parlist);
    j["vargs"] = false;

    if (self->namelist && self->vargs) {
        JSON_SUB(namelist);
        j["vargs"] = true;
    } else if (self->namelist) {
        JSON_SUB(namelist);
    } else {
        j["vargs"] = true;
    }
    return j;
}

PRINT(prefixexp)
{
    DJ(prefixexp);
    if (self->var) {
        JSON_SUB(var);
    } else if (self->functioncall) {
        JSON_SUB(functioncall);
    } else {
        JSON_SUB(exp);
    }
    return j;
}

PRINT(stat)
{
    DJ(stat);
    switch (self->type) {
        case stat_type_varlist_assgin_explist:
            j["subType"] = "varlist_assgin_explist";
            JSON_SUB(varlist);
            JSON_SUB(explist);
            break;
        case stat_type_functioncall:
            j["subType"] = "functioncall";
            JSON_SUB(functioncall);
            break;
        case stat_type_do_block_end:
            j["subType"] = "do_block_end";
            JSON_SUB(block);
            break;
        case stat_type_repeat_block_until_end:
            j["subType"] = "repeat_block_until";
            JSON_SUB(block);
            JSON_SUB(exp);
            break;
        case stat_type_if:
            j["subType"] = "if";
            JSON_SUB(exp);
            JSON_SUB(block);
            JSON_SUB(elseiflist);
            JSON_SUB(elsepart);
            break;
        case stat_type_for_loop:
            j["subType"] = "for_loop";
            j["name"] = self->name;
            JSON_SUB(exp0);
            JSON_SUB(exp1);
            JSON_SUB(block);
            break;
        case stat_type_for_loop_step:
            j["subType"] = "for_loop_step";
            j["name"] = self->name;
            JSON_SUB(exp0);
            JSON_SUB(exp1);
            JSON_SUB(exp2);
            JSON_SUB(block);
            break;
        case stat_type_for_in:
            j["subType"] = "for_in";
            JSON_SUB(namelist);
            JSON_SUB(explist);
            JSON_SUB(block);
            break;
        case stat_type_function:
            j["subType"] = "function";
            JSON_SUB(funcname);
            JSON_SUB(funcbody);
            break;
        case stat_type_local_function:
            j["subType"] = "local_function";
            j["name"] = self->name;
            JSON_SUB(funcbody);
            break;
        case stat_type_local_namelist:
            j["subType"] = "local_namelist";
            JSON_SUB(namelist);
            break;
        case stat_type_local_namelist_assgin_explist:
            j["subType"] = "local_namelist_assgin_explist";
            JSON_SUB(namelist);
            JSON_SUB(explist);
            break;
        case stat_type_while:
            j["subType"] = "while";
            JSON_SUB(exp);
            JSON_SUB(block);
            break;
    }
    return j;
}

PRINT(statlist)
{
    DJ(statlist);
    JSON_SUB(statlist);
    JSON_SUB(statpart);
    return j;
}

PRINT(statpart)
{
    DJ(statpart);
    JSON_SUB(stat);
    j["semicolon"] = self->semicolon == 1;
    return j;
}

PRINT(tableconstructor)
{
    DJ(tableconstructor);
    JSON_SUB(fieldlist);
    return j;
}

PRINT(var)
{
    DJ(var);
    if (self->prefixexp) {
        JSON_SUB(prefixexp);
        if (self->exp) {
            JSON_SUB(exp);
        } else {
            j["name"] = self->name;
        }
    } else {
        j["name"] = self->name;
    }
    return j;
}

PRINT(varlist)
{
    DJ(varlist);
    if (self->varlist) {
        JSON_SUB(varlist);
    }
    JSON_SUB(var);
    return j;
}



int main()
{
    nx::pipe::source in(std::cin);

    std::stringstream ss;
    nx::pipe::sink out(ss);

    in | out;

    auto code = ss.str();

    block_t* block = luam_parse((char*)code.c_str());
    auto j = print_ast_root(block, std::cout);
    destroy_block(block);

    std::cout << j.dump();

    return 0;

}