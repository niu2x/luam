C_FLAGS := -g -DYYDEBUG=1
HEADER_PATHS := -I/home/niu2x/project/niu2x/include \
				-I/home/niu2x/project/niu2x/build
LD_FLAGS := -L//home/niu2x/project/niu2x/build/lib -lniu2x
CPP_FLAGS := -std=c++17

all: luam

objs := token_name.o \
	lua.yy.o \
	ast.o \
	main.o \
	ast_printer.o \
	lua.tab.o

luam: ${objs}
	g++ -o $@ $^ ${LD_FLAGS}	

${objs}: lua.tab.h

%.o: %.c
	gcc -c ${C_FLAGS} ${HEADER_PATHS} $< -o $@

%.o: %.cpp
	g++ -c ${C_FLAGS} ${HEADER_PATHS} ${CPP_FLAGS} $< -o $@

%.tab.c %.tab.h: %.y
	bison -d $<

%.yy.c: %.l
	flex -o $@ $<


clean:
	rm -rf *.o
	rm -rf luam
	rm -rf *.yy.*
	rm -rf *.tab.*
