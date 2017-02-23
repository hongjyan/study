#include <unistd.h>
#include "lib.hpp"
#include <cstdio>
	
void Context::daemonFunc() {
	while(true) {
    	if (nullptr != Context::eventHandler_) {
        	printf("eventHandler_ is not NULL\n");
			Context::eventHandler_(1);
		}
		else {
			printf("eventHandler_ is null still\n");
		}
		sleep(1);
	}
}

Context::Context(EventHandler handler): eventHandler_(handler) {
	printf("eventHandler_ is %p\n", eventHandler_);
	eventThread_ = std::thread(&Context::daemonFunc, this);
	eventThread_.detach();
	printf("eventThread_.joinable() is %d\n", eventThread_.joinable());
} 

Context::~Context() {
	//eventHandler_ = nullptr;
	printf("eventThread_.joinable() is %d\n", eventThread_.joinable());
	Context::eventThread_.join();
}

void Context::setEventHandler(EventHandler eventHandler) {
	printf("eventHandler_ is %p\n", eventHandler_);
	eventHandler_ = eventHandler;
	printf("eventHandler_ is %p after set\n", eventHandler_);
} 

