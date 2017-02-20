#include "lib.hpp"
#include  "stub.hpp"
#include <iostream>

void foo(int i) {
	printf("i is %d\n", i);
}

int main() {
	Stub stub;
	Context ctx;
//	Context ctx2(stub.callback);
	ctx.setEventHandler(stub.callback);
	return 0;
}
	
