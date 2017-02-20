#include <thread>
#include <pthread.h>

typedef void (*EventHandler)(int);
 
class Context {
public:
	std::thread eventThread_;
	void daemonFunc();

public:
	EventHandler eventHandler_;
	Context(EventHandler handler=nullptr);
	~Context();
	void setEventHandler(EventHandler eventHandler);

};
    
