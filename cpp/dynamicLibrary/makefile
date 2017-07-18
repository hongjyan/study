all: libPrintf test.o
include makerpm2.mk
test.o: test.cpp test test2
	g++ -Wall -ggdb -L. -lPrintf -ldl test.cpp -o test.o 
libPrintf: Printf.cpp
	g++ -fPIC -shared  Printf.cpp -o libPrintf.so
clean:
	rm -rf *.o *.so

include makerpm.mk
