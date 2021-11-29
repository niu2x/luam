C_FLAGS := -g -DYYDEBUG=1
LD_FLAGS := 

all: luam

objs := token_name.o \
	lua.yy.o \
	ast.o \
	lua.tab.o

luam: ${objs}
	gcc -o $@ $^ ${LD_FLAGS}	

${objs}: lua.tab.h

%.o: %.c
	gcc -c ${C_FLAGS} $< -o $@

%.tab.c %.tab.h: %.y
	bison -d $<

%.yy.c: %.l
	flex -o $@ $<