extern "C" {
#include "ast.h"
}

#include "ast_printer.h"

#include <string>
#include <variant>
#include <sstream>
#include <niu2x/pipe.h>

int main(){
	nx::pipe::source in(std::cin);

	std::stringstream ss;
	nx::pipe::sink out(ss);

	in | out;

	auto code = ss.str();

	block_t *block =  luam_parse((char*)code.c_str());
	if(block){
		print_ast_root(block, std::cout);
		destroy_block(block);
	}
	return 0;
}