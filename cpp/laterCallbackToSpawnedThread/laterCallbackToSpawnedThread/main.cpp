#include "lib.hpp"
#include  "stub.hpp"
#include <iostream>
#include <unistd.h>

void foo(int i) {
	printf("i is %d\n", i);
}

void wrap() {
	Stub stub;
	Context ctx;
//	sleep(1);
	ctx.setEventHandler(stub.callback);
}

int main() {
    Context ctx;
    Stub stub;
	//wrap();

    ctx.setEventHandler(stub.callback);
//	Stub stub;
//	Context ctx;
	//sleep(10);
	//ctx.setEventHandler(nullptr);

//
//    sleep(30);
	return 0;
}
	
