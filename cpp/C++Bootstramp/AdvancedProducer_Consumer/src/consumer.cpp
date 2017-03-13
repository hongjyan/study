#include "consumer.hpp"
#include <iostream>
#include <unistd.h>
#include "producer.hpp"
#include <cstdio>
#include <stdexcept>

namespace RT {
    Consumer::Consumer(const std::string& id): ID_(id), start_(false), stop_(false), pProducer_(NULL) { log_.setMouduleName(ID_); }

    Consumer::~Consumer() { deregisterProducer(); }

    void Consumer::start() { start_ = true; }

    void Consumer::stop() { stop_ = start_ ? true : false; }

    void Consumer::run() {
        std::pair<int, int> operators;
        float result;
#ifdef DEBUG
        int i = 0;
#endif        
        while (true) {
            if (!start_) {
                usleep(100);
            }

            if (stop_) {
                return;
            }

            if (NULL == pProducer_) {
#ifdef DEBUG
                log_.log(LOG_INFO, "pProducer_ is null\n");
#endif
                usleep(100);
            }
#ifdef DEBUG
            log_.log(LOG_INFO, "My ID is %s, i is %d, start_ is %d, pProducer_ is %p\n", ID_.c_str(), i, start_, pProducer_);
            i++;
#endif
            if (true == pProducer_->distributeDividOperators(*this, operators)) {
                try {
                    result = initDivEx(operators.first, operators.second);
                }
                catch (std::overflow_error& e) {
                    std::cout << e.what() << std::endl;
                    //throw e;
                }
                log_.log(LOG_INFO, "numerator is %d, denominator is %d, result is %f\n", operators.first, operators.second, result);
            }

            usleep(1000);
        }
    }

    bool Consumer::registerProducer(Producer* pProducer)
    {
        if (NULL == pProducer) {
#ifdef DEBUG
            log_.log(LOG_INFO, "ID:%s should not come here\n", ID_.c_str());
#endif
            return false;
        }

        pProducer_ = pProducer;
#ifdef DEBUG
        log_.log(LOG_INFO, "ID:%s register producer\n", ID_.c_str());
#endif      
        pProducer_->registerConsumer(*this);
        return true;
    }

    void Consumer::deregisterProducer()
    {
        stop();
        pProducer_->deRegisterConsumer(*this);
        pProducer_ = NULL;
    }

    float Consumer::initDivEx(int numerator, int denominator) {
        if (0 == denominator) {
            throw std::overflow_error("denominator is 0, will cause undefined behavior, throw over_flowerror");
        }
        else {
            return (float)numerator/denominator;
        }
    }
}
