#ifndef __ZMQAPPSERVER_HPP__
#define __ZMQAPPSERVER_HPP__
#include "../AppCommon.hpp"
#include "../../zmq/zmqServer.hpp"

class ZMQAppServer : public AppCommon {
public:
    ZMQAppServer(int argc, char* argv[]);
    ~ZMQAppServer();

    void run();
private:
    ZMQServer zmqServer_;
};
#endif
