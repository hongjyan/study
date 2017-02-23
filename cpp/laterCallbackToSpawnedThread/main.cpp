#include "lib.hpp"
#include  "stub.hpp"
#include <iostream>
#include <unistd.h>

void foo(int i) {
	printf("i is %d\n", i);
}


int main() {
    Context ctx;
    Stub stub;

    ctx.setEventHandler(stub.callback);

	//pthread_exit(0); //pthread_exit still can not prevent resource of sub-thread released
	return 0;
}
	
