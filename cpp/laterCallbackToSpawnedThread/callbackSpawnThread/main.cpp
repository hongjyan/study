#include "lib.hpp"
#include  "stub.hpp"
#include <iostream>
#include <unistd.h>

void foo(int i) {
	printf("i is %d\n", i);
}


int main() {
	Context ctx;
	sleep(1);
	Stub stub;  //no matter stub is created before or after spawned thread of Ctx, it is ok to assign stub.callback to eventHandler_ of Ctx, new eventHandler_ is visiable to spawn thread.
	ctx.setEventHandler(stub.callback);
	sleep(10);
	return 0;
}
	
