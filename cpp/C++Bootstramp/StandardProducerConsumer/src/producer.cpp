#include <unistd.h>
#include "producer.hpp"
//#include <random>

RT::Producer::Producer(unsigned char maxNum, unsigned int size): maxNum_(maxNum), running_(false), maxSize_(size)
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_ROBUST);
    pthread_mutex_init(&queueMutex_, &attr);
    log_.setMouduleName("Producer");
}

RT::Producer::~Producer() {
    running_ = false;
}

void RT::Producer::run() {
    while (true) {
        if (!running_) {
            usleep(1000);
            continue;
        }

        if (pthread_mutex_lock(&queueMutex_)) {
            throw std::exception();
        }

        if (queue_.size() > maxSize_) {
            usleep(1000);
            if (pthread_mutex_unlock(&queueMutex_)) {
                throw std::exception();
            }
            continue;
        }

        if (pthread_mutex_unlock(&queueMutex_)) {
            throw std::exception();
        }

        std::pair<unsigned int, unsigned int> operators = generateRandomOperators();
        queue_.push(operators);
        log_.log(LOG_INFO, "Push [%u, %u]\n", operators.first, operators.second);

        usleep(1000);
    }
}

void RT::Producer::start()
{
    running_ = true;
}

void RT::Producer::stop()
{
    running_ = false;
}

bool RT::Producer::distributeData(std::pair<unsigned int, unsigned int>& operators)
{
    if (pthread_mutex_lock(&queueMutex_)) {
        throw std::exception();
    }
    if (queue_.empty()) {
        if (pthread_mutex_unlock(&queueMutex_)) {
            throw std::exception();
        }
        return false;
    }

    std::pair<unsigned int, unsigned int> element = queue_.front();
    queue_.pop();
    if (pthread_mutex_unlock(&queueMutex_)) {
        throw std::exception();
    }
    operators.first = element.first;
    operators.second = element.second;
    return true;
}

std::pair<unsigned int, unsigned int> RT::Producer::generateRandomOperators()
{
    /*
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, maxNum_);
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


