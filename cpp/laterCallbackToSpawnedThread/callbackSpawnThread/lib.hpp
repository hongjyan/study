#include <thread>
#include <pthread.h>

typedef void (*EventHandler)(int);
 
class Context {
private:
	std::thread eventThread_;
	void daemonFunc();
	EventHandler eventHandler_;

public:
	Context(EventHandler handler=nullptr);
	~Context();
	void setEventHandler(EventHandler eventHandler);

};
    
