HEADER_PATHS := -I../lua51 -I../luam

LD_FLAGS := -llua -lniu2x -L../lua51 -lcrypto -L/usr/local/lib64 -ldl -lluam -L../luam

CPP_FLAGS := -std=c++17

astdump: main.o
	g++ -o $@ $^ $(LD_FLAGS)	

%.o: %.c
	gcc -c $(C_FLAGS) $(HEADER_PATHS) $< -o $@

%.o: %.cpp
	g++ -c $(HEADER_PATHS) $(CPP_FLAGS) $< -o $@

clean:
	rm -f *.o
	rm -f astdump