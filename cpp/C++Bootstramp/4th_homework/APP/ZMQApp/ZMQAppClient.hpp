#ifndef __ZMQAPPCLIENT_HPP__
#define __ZMQAPPCLIENT_HPP__
#include "../AppCommon.hpp"
#include "../../zmq/zmqClient.hpp"

class ZMQAppClient : public AppCommon {
public:
    ZMQAppClient(int argc, char* argv[]);
    ~ZMQAppClient();
    void run();
private:
    ZMQClient zmqClient_;
};
#endif
