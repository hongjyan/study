#include <unistd.h>
#include <cstdio>
#include <thread>
#include <pthread.h>

typedef void (*EventHandler)(int);
 
class Context {
public:
    Context(EventHandler handler = nullptr) {
        eventHandler_ = handler;
        eventThread_ = std::thread(&Context::daemonFunc, this);
        eventThread_.detach();
    };
    
 
    ~Context() {
        eventHandler_ = nullptr;// wrong, since this will be called quick when main finshed.
        //eventThread_.join();  //call join after detach will let program crash!!!!
    }
    
    void setEventHandler(EventHandler eventHandler) {
         eventHandler_ = eventHandler;
    }


    void daemonFunc() {
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
private:
     EventHandler eventHandler_;
     std::thread eventThread_;
};

class  Stub {
public:
    static void callback(int i) {
        printf("i is %d\n", i);
    }
};



int main() {
    Context ctx;
    ctx.setEventHandler(Stub::callback);
    pthread_exit(0);
    printf("after pthread_exit\n");
    return 0;
}
