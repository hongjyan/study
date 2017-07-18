#ifndef _PRODUCER_HPP_
#define _PRODUCER_HPP_

#include <queue>
//#include <mutex>
#include "log.hpp"
#include "runnable.hpp"
#include <pthread.h>
#include <cstdlib>

namespace RT {
class Producer : public Runnable {
public:
    Producer(unsigned char maxNum, unsigned int size=99);
    ~Producer();
    void run();
    void start();
    void stop();
    bool distributeData(std::pair<unsigned int, unsigned int>& OutOperators);
private:
    unsigned char maxNum_;
    bool running_;
    unsigned int maxSize_;
    std::queue<std::pair<unsigned int, unsigned int> > queue_;
    pthread_mutex_t queueMutex_;
    Log log_;
    std::pair<unsigned int, unsigned int> generateRandomOperators();
};

}

#endif
