#include <unistd.h>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <utility>
#include <list>
#include <exception>
#include <cstring>
#include "producer.hpp"
#include "log.hpp"
#include "consumer.hpp"

namespace RT {
//define nested Pop class
    Producer::Pop::Pop(Producer& producer): r_producer(producer), start_(false), stop_(false) {}
    Producer::Pop::~Pop()
    {
    }

    void Producer::Pop::run()
    {
        while (true) {
            if (!start_) {
                usleep(100);
            }
        
            if (stop_) {
                return;
            }
            
#ifdef DEBUG
            int i = 0;
#endif

            if (0 != pthread_mutex_lock(&(r_producer.mutex_))) {
                r_producer.log_.log(LOG_ERROR, "Lock mutex in Producer::Pop::run() failed\n");
                throw std::exception();
            } else {
#ifdef DEBUG
                r_producer.log_.log(LOG_INFO, "Acquire mutex in Pop success\n");
#endif
            }

            for (std::list<Data>::iterator it = r_producer.queue_.begin(); it != r_producer.queue_.end(); ++it) {
#ifdef DEBUG
//                r_producer.log_.log(LOG_ERROR, LOG_INFO,"i is %d, r_producer.queue_.size() is %zd, it->IDs.size() is %lu, r_producer.IDs.size() is %lu\n",
//                       i, r_producer.queue_.size(), it->IDs.size(), r_producer.IDs.size());
                i++;
#endif
                if (it->IDs.size() == r_producer.IDs.size() && 0 != r_producer.IDs.size()) { //should improve. Say User2 in IDs, User1 not in, User2 stop.
                    //then,this element maybe poped even User1 have not consumed it.

                    r_producer.log_.log(LOG_INFO, "Pop [%d,%d]\n", it->numerator_denominator.first, it->numerator_denominator.second);
                    r_producer.queue_.erase(it);
                    --it;
                }
            }

            if (0 != pthread_mutex_unlock(&(r_producer.mutex_))) {
                r_producer.log_.log(LOG_ERROR, "Unlock mutex in Producer::Pop::run() failed\n");
                throw std::exception();
            } else {
#ifdef DEBUG
                r_producer.log_.log(LOG_INFO,"unlock mutex in Pop success\n");
#endif
            }

            usleep(1000);
        }
    }

 //define nested Push class
     void Producer::Push::run()
     {
         while (true) {
             if (!start_) {
                 usleep(100);
             }

             if (stop_) {
                 return;
             }

             if (pthread_mutex_lock(&r_producer.mutex_)) {
                 r_producer.log_.log(LOG_ERROR, "Lock mutex in Producer::Push::run() failed\n");
                 throw std::exception();
             }

             if (max_ < r_producer.queue_.size()) { //size() will iterator all element of list, that will cause carsh if pop
                 //thread erase element. WTF.
                 r_producer.log_.log(LOG_INFO, "queue reach maximum, wait for 1000us to push\n");
                 usleep(1000);
             } 

             if (pthread_mutex_unlock(&r_producer.mutex_)) {
                 r_producer.log_.log(LOG_ERROR, "Unlock mutex in Producer::Push::run() failed\n");
                 throw std::exception();
             }

             std::pair<int, int> operators = generateRandomOperators();
             std::list<std::string> lst;
             Data data;
             data.numerator_denominator = operators;
             data.IDs = lst; 
             r_producer.queue_.push_back(data);
             r_producer.log_.log(LOG_INFO,"Pushed [%d,%d]\n", operators.first, operators.second);
             usleep(1000);
         }
     }

     std::pair<int, int> Producer::Push::generateRandomOperators()
     {
     /*
         std::mt19937 rng;
         rng.seed(std::random_device()());
         std::uniform_int_distribution<std::mt19937::result_type> dist(0, max_);
         return std::make_pair(dist(rng), dist(rng));
     */

         static int i;
         i += 100;
         srand(time(NULL)+i);
         std::pair<int, int> temp;
         temp.first = std::rand()%10;
         temp.second = std::rand()%10;
         return temp;

     }

 //define Producer class
    Producer::Producer(unsigned int max, unsigned short queueSize): max_(max), queueSize_(queueSize),
              pop_(*this), push_(max_, *this)
    {
        pthread_mutexattr_t attr;
        if (0 != pthread_mutexattr_init(&attr) || 0 != pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_ROBUST)) {
            log_.log(LOG_ERROR, "set robust for attribute of mutex failed in Producer::Producer\n");
            throw std::exception();
        }


        if (0 != pthread_mutex_init(&mutex_, &attr)) {
            log_.log(LOG_ERROR, "init mutex failed in Producer::Producer\n");
            throw std::exception();
        }

        log_.setMouduleName("Producer");
    }

    Producer::~Producer()
    {
        pthread_mutex_destroy(&mutex_);
        stop(false);
    }

    void Producer::start()
    {
        pop_.start();
        push_.start();

        Thread popThread(&pop_);
        Thread pushThread(&push_);
    }

    void Producer::stop(bool atonce)
    {
        if (atonce) {
            push_.stop();
            pop_.stop();
        }
        else {
            push_.stop();
            sleep(5); // then sleep 5s
            pop_.stop(); //then stop pop.
        }
    }

    void Producer::registerConsumer(const Consumer& consumer)
    {
        IDs.push_back(consumer.getID());
    }

    void Producer::deRegisterConsumer(const Consumer& consumer)
    {
       std::list<ID>::iterator it = find(IDs.begin(), IDs.end(), consumer.getID());
       if (IDs.end() != it) {
           IDs.erase(it);
       }
    }


    bool Producer::distributeDividOperators(Consumer& consumer, std::pair<int, int>& outOperators)
    {
        ID id = consumer.getID();
        bool distributed = false;

        if (0 != pthread_mutex_lock(&mutex_)) {
            log_.log(LOG_ERROR, "lock mutex in Producer::distributeDividOperator failed\n");
            throw std::exception();
        } else {
#ifdef DEBUG
            log_.log(LOG_INFO,"Acquire mutex success by %s\n", id.c_str());
#endif
        }

        std::list<Data>::iterator qit = queue_.begin();
        while (queue_.end() != qit) {
            distributed = false;
            for (std::list<ID>::iterator it = qit->IDs.begin(); it != qit->IDs.end(); ++it) {
                if (id == *it) {
                    distributed = true;
                    break;
                }
            }

            if (!distributed) {
#ifdef DEBUG
                log_.log(LOG_INFO,"Push ID %s\n", id.c_str());
#endif
                outOperators = qit->numerator_denominator;
                qit->IDs.push_back(id);

                if (0 != pthread_mutex_unlock(&mutex_)) {
                    log_.log(LOG_ERROR, "unlock mutex in Producer::distributeDividOperator failed\n");
                    throw std::exception();
                } else {
#ifdef DEBUG
                    log_.log(LOG_INFO,"Unlock mutex success by %s\n", id.c_str());
#endif
                }

                return true;
            }

#ifdef DEBUG
            log_.log(LOG_INFO,"ID:%s already serviced\n", id.c_str());
#endif
            qit++;
        }

        if (0 != pthread_mutex_unlock(&mutex_)) {
            log_.log(LOG_ERROR, "unLock mutex in Producer::distributeDividOperator failed\n");
            throw std::exception();
        } else {
#ifdef DEBUG
            log_.log(LOG_INFO,"Unlock mutex success by %s\n", id.c_str());
#endif
        }


        return false;
    }

}

