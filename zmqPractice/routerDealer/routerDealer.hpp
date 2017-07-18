#ifndef __ROUTERDEALER_HPP__
#define __ROUTERDEALER_HPP__
#include <zmq.h>
#include <cstdio>
#include <cstring>

#define ROUTERADDR "tcp://127.0.0.1:12345"
#define DEALERADDR "tcp://127.0.0.1:54321"
class RouterDealer {
public:
    RouterDealer();
    ~RouterDealer();
    bool run();
private:
    void *ctx_, *zmqrouter_, *zmqdealer_;
};
#endif
