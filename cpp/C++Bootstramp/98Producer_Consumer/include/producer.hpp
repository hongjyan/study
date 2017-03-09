#ifndef PRODUCER_HPP
#define PRODUCER_HPP
#include <string>
#include <list>
#include <utility>
#include <pthread.h>
#include "runnable.hpp"
#include "log.hpp"



namespace RT {
typedef std::string ID;
class Consumer;

class Producer {
struct Data {
	std::pair<int, int> numerator_denominator;
	std::list<ID> IDs; //no need direct access element, need erase. So use list
};

public:
	Producer(unsigned int max=10, unsigned short queueSize=120);
	~Producer();

	unsigned int getSeed() { return max_; }
	void setSeed(unsigned int max) { max_ = max; }

	unsigned short getQueueSize() { return queueSize_; }
	void setQueueSize(unsigned short queueSize) { queueSize_ = queueSize; }

	void start();
	void stop(bool atonce);

	void registerConsumer(const Consumer& consumer);
	void deRegisterConsumer(const Consumer& consumer);

	bool distributeDividOperators(Consumer& consumer, std::pair<int, int>& outOperators);

	void updateQueue(Consumer& consumer); //update the ID of first data in queue who do not has pConsumer->id;
private:
    class Pop : public Runnable {
    public:
        Pop(Producer& producer);
        ~Pop();

        void run();

        void start() { start_ = true; }
    
        void stop() { stop_ = start_ ? true : false; }
    private:
        Producer& r_producer;
        bool start_, stop_;
    };

    class Push : public Runnable {
    public:
        Push(unsigned int max, Producer& producer): start_(false), stop_(false), max_(max), r_producer(producer) {}
        ~Push() { stop(); }
        void run();
        void start() { start_ = true; }
        void stop() { stop_ = start_ ? true : false; }

    private:
        bool start_, stop_;
        unsigned int max_;
        Producer& r_producer;
        std::pair<int, int> generateRandomOperators();

    };

    unsigned int max_;
    unsigned short queueSize_;
	Pop pop_;
	Push push_;
    //std::mutex QueueMutex_;
	std::list<Data> queue_; //no need direct access to random element, but need erasing element frequently, so use list.
	std::list<ID> IDs; //assume ID is unique for each consumer
    pthread_mutex_t mutex_;
    Log log_;
};
}

#endif
