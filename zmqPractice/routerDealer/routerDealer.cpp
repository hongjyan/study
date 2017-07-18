#include "routerDealer.hpp"

RouterDealer::RouterDealer()
{
    ctx_ = 0; zmqrouter_ = 0; zmqdealer_ = 0;
}

RouterDealer::~RouterDealer()
{
    if (zmqrouter_) {
        zmq_close(zmqrouter_);
        zmqrouter_ = 0;
    }

    if (zmqdealer_) {
        zmq_close(zmqdealer_);
        zmqdealer_ = 0;
    }

    if (ctx_) {
        zmq_ctx_destroy(ctx_);
        ctx_ = 0;
    }
}

bool RouterDealer::run()
{
    ctx_ = zmq_ctx_new();
    if (0 == ctx_) return false;

    zmqrouter_ = zmq_socket(ctx_, ZMQ_ROUTER);
    if (0 == zmqrouter_) return false;

    zmqdealer_ = zmq_socket(ctx_, ZMQ_DEALER);
    if (0 == zmqdealer_) return false;

    if (-1 == zmq_bind(zmqrouter_, ROUTERADDR)) {
        printf("bind zmqrouter failed for %s, reason is %s\n",ROUTERADDR, strerror(errno));
        return false;
    }

    if (-1 == zmq_bind(zmqdealer_, DEALERADDR)) {
        printf("bind zmqdealer failed for %s, reason is %s\n", DEALERADDR, strerror(errno));
        return false;
    }

    if (-1 == zmq_proxy(zmqrouter_, zmqdealer_, 0)) {
        printf("zmq_proxy failed, reasin is %s\n", strerror(errno));
        return false;
    }

    return true;
}
