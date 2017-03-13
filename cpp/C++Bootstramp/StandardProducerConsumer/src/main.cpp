#include "producer.hpp"
#include "consumer.hpp"
#include "runnable.hpp"
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include "log.hpp"


int main() {
    Log log;
    log.setMouduleName("Main");
    RT::Producer KFC(10, 100);
    RT::Consumer xiaoming("xiaoming"), xiaoli("xiaoli");



    RT::Thread thd0(&KFC);
    RT::Thread thd1(&xiaoming);
    RT::Thread thd2(&xiaoli);
    xiaoming.registerProducer(&KFC);
    xiaoli.registerProducer(&KFC);
    KFC.start();
    xiaoming.start();
    xiaoli.start();

    log.log(LOG_INFO, "1--------------\n");
    sleep(10);
    log.log(LOG_INFO, "2--------------\n");
    KFC.stop();
    xiaoming.stop();
    xiaoli.stop();
    log.log(LOG_INFO, "3--------------\n");
    return 0;

    pthread_exit(0);

}
