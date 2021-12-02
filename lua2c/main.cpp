#include <iostream>
#include <sstream>

extern "C" {

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

}

#include <niu2x/pipe.h>

static std::string read_input() {
	nx::pipe::source source(std::cin);
	std::stringstream ss;
	nx::pipe::sink sink(ss);	
	source | sink;
	return ss.str();
}

static int panicf (lua_State *L) {
	auto *sz = lua_tostring(L, -1);
	std::cerr << sz << std::endl;
	return 0;
}

int main(int argc, char *argv[]){
	std::string sz;
	auto *L = luaL_newstate();

	lua_atpanic (L, panicf);

	luaL_openlibs(L);

	if(argc >= 2) {
		sz = argv[1];
		auto ret = luaL_dofile(L, sz.c_str());
		if(ret != 0){
		  fprintf(stderr, "Error: %s\n", lua_tostring(L,-1));
		}
	}
	else{
		sz = read_input();
		auto ret = luaL_dostring(L, sz.c_str());
		if(ret != 0){
		  fprintf(stderr, "Error: %s\n", lua_tostring(L,-1));
		}
	}

	lua_close(L);
	return 0;
}