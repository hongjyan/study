#ifndef CONSUMER_HPP
#define CONSUMER_HPP
#include <utility>
#include "runnable.hpp"
#include <string>
#include "log.hpp"

namespace RT {
struct Result {
    std::pair<int, int> numerator_denominator;
    float result;
};
class Producer;
class Consumer : public Runnable {
public:
    Consumer(const std::string& id); //id should be unique among all users
    ~Consumer();
    inline std::string getID() const { return ID_; }
	void run();
	void start();
	void stop();
	bool registerProducer(Producer* pProducer);
	void deregisterProducer();
	void consuming();
private:
	float initDivEx(int numerator, int denominator);
	std::string ID_;
	bool start_, stop_;
	Producer* pProducer_; //producer can be registered at any time, so this variable is point-type. Reference-type need be associated at beginning.
    Log log_;
};
}
#endif
