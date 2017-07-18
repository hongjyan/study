#include <stdio.h>
extern "C" {    //disable C++ mangled

    void Printf(void) {
	printf("I am Printf\n");
    }
}
