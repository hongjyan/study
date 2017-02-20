#include <unistd.h>
#include "lib.hpp"
#include <cstdio>

//EventHandler Context::eventHandler_ = nullptr;
//std::thread Context::eventThread_ = std::thread();
	
void Context::daemonFunc() {
	while(true) {
    	if (nullptr != eventHandler_) {
        	printf("eventHandler_ is not NULL\n");
			eventHandler_(1);
		}
		else {
			printf("eventHandler_ is null still\n");
		}
		sleep(1);
	}
}

Context::Context(EventHandler handler) {
    eventHandler_ = handler;
	printf("eventHandler_ is %p\n", eventHandler_);
    eventThread_ = std::thread(&Context::daemonFunc, this);
	//Context::eventThread_.detach();
} 

Context::~Context() {
	//eventHandler_ = nullptr;
	eventThread_.join();
}

void Context::setEventHandler(EventHandler eventHandler) {
	printf("eventHandler_ is %p\n", eventHandler_);
	eventHandler_ = eventHandler;
	printf("eventHandler_ is %p after set\n", eventHandler_);
} 

