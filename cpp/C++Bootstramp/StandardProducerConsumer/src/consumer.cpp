#include "consumer.hpp"
#include <unistd.h>
#include "producer.hpp"
#include <stdexcept>

RT::Consumer::Consumer(const std::string& name): name_(name), running_(false), pProducer_(NULL)
{
    log_.setMouduleName(name);
}

RT::Consumer::~Consumer() {
    stop();
    pProducer_ = NULL;

}

bool RT::Consumer::registerProducer(Producer* pProducer)
{
    if (NULL == pProducer) {
        return false;
    }

    pProducer_ = pProducer;
    return true;
}

void RT::Consumer::deregisterProducer()
{
    pProducer_ = NULL;
}

void RT::Consumer::start()
{
    running_ = true;
}

void RT::Consumer::stop()
{
    running_ = false;
}

void RT::Consumer::run()
{
    while (true) {
        if (!running_) {
            usleep(1000);
            continue;
        }

        if (NULL == pProducer_) {
            log_.log(LOG_INFO, "Still not register producer, please register a producer\n");
        }

        std::pair<unsigned int, unsigned int> operators;
        if (false == pProducer_->distributeData(operators)) {
            log_.log(LOG_INFO, "No element in producer, wait 1ms for next try\n");
        }
        else {
            float result;
            try {
                result = initDivEx(operators.first, operators.second);
            } catch (std::overflow_error& e) {
                log_.log(LOG_ERROR, "%s\n", e.what());
                usleep(5000);
                continue;
            }
            log_.log(LOG_INFO, "numerator is %u, denominator is %u, result is %.2f\n",
                    operators.first, operators.second, result);
        }
        usleep(5000);

    }
}

float RT::Consumer::initDivEx(int numerator, int denominator)
{
    if (0 == denominator) {
        throw std::overflow_error("denominator is 0, will cause undefined behavior, throw overflow_error\n");
    }
    else {
        return (float)numerator/denominator;
    }
}


