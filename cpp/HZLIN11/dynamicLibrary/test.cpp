//#include "Printf.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

void Printf() {
    printf("I am Printf in test.cpp\n");
}

int main(int argc, char* argv[]) {
    void* lib_handler = dlopen("libPrintf.so", RTLD_LAZY|RTLD_LOCAL);
    if (!lib_handler) {
	printf("[%s] unable to load library: %s\n", __FILE__, dlerror());
	exit(EXIT_FAILURE);
    }

    void* p = dlsym(lib_handler, "Printf");
    if (!p) {
	printf("[%s] unable to get symbol: %s\n", __FILE__, dlerror());
	exit(EXIT_FAILURE);
    }
    void (*Printf)() = (void (*) (void))p;

    Printf();

    dlclose(lib_handler);
}
