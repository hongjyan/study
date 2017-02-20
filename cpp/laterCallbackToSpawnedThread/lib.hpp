#include <thread>
#include <pthread.h>

using EventHandler = void (*)(int);
 
class Context {
private:
	std::thread eventThread_;
	EventHandler eventHandler_;
	void daemonFunc();

public:
	Context(EventHandler handler=nullptr);
	~Context();
	void setEventHandler(EventHandler eventHandler);

};
    
