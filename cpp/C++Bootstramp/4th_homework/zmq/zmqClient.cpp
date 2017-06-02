#include "zmqClient.hpp"
#include <cstdio>
#include <sstream>

ZMQClient::ZMQClient(): ZMQCnx()
{
    zmqSocket_ = zmq_socket(zmqContext_, ZMQ_DEALER);
}

ZMQClient::~ZMQClient()
{
    zmq_close(zmqSocket_);
}

bool ZMQClient::start(const char* localaddr, unsigned short lport, const char* remoteaddr, unsigned short rport)
{
    bool bRet = false;
    std::ostringstream oss;
    oss << "tcp://" << remoteaddr << ":" << (int)rport;

    if (0 != zmq_connect(zmqSocket_, oss.str().c_str())) {
        printf("ZMQClient connect to %s failed\n", oss.str().c_str());
        return false;
    } else {
        printf("ZMQClient connect to %s succeed\n", oss.str().c_str());
    }

    bRet = startThread();

    if (bRet) {
        printf("Start ZMQClient succeed\n");
    } else {
        printf("Start ZMQClient failed\n");
    }

    return bRet;
}


