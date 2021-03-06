extern "C" {
#include "ast.h"
}

#include "ast_printer.h"

#include <niu2x/pipe.h>
#include <sstream>
#include <string>
#include <variant>

int main()
{
    nx::pipe::source in(std::cin);

    std::stringstream ss;
    nx::pipe::sink out(ss);

    in | out;

    auto code = ss.str();

    block_t* block = luam_parse((char*)code.c_str());
    print_ast_root(block, std::cout);
    destroy_block(block);
    return 0;
}