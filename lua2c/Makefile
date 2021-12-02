HEADER_PATHS := -I../lua51/

lua2c: main.o
	g++ -o $@ $^ $(LD_FLAGS)	

%.o: %.c
	gcc -c $(C_FLAGS) $(HEADER_PATHS) $< -o $@

%.o: %.cpp
	g++ -c $(C_FLAGS) $(HEADER_PATHS) $(CPP_FLAGS) $< -o $@
