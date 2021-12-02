#ifndef LUAM_AST_PRINTER_H
#define LUAM_AST_PRINTER_H

extern "C" {
#include "ast.h"
}

#include <ostream>

void print_ast_root(const block_t* block, std::ostream& os);

#endif