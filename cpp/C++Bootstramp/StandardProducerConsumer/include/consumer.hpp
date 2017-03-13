#ifndef _CONSUMER_HPP_
#define _CONSUMER_HPP_
#include <string>
#include "log.hpp"
#include "runnable.hpp"


namespace RT {
class Producer;
class Consumer : public Runnable {
public:
    Consumer(const std::string& name);
    ~Consumer();
    bool registerProducer(Producer* pProducer);
    void deregisterProducer();
    void start();
    void stop();
    void run();
private:
    std::string name_;
    bool running_;
    Log log_;
    Producer* pProducer_;
    float initDivEx(int, int);
};

}

#endif
