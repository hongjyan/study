#include "zmqServer.hpp"
#include <sstream>
#include <cstring>

ZMQServer::ZMQServer() : ZMQCnx()
{
    zmqSocket_ = zmq_socket(zmqContext_, ZMQ_DEALER);
}

ZMQServer::~ZMQServer()
{
    zmq_close(zmqSocket_);
}

bool ZMQServer::start(const char* localaddr, unsigned short lport, const char* remoteaddr, unsigned short rport)
{
    bool ret = false;
    std::ostringstream oss;
    oss << "tcp://" << localaddr << ":" << (unsigned int)lport;
    if (0 != zmq_bind(zmqSocket_, oss.str().c_str())) {
        printf("zmq_bind to %s failed", oss.str().c_str());
        return ret;
    }

    ret = startThread();

    if (ret)
       printf("ZMQServer start succeed\n");
    else
        printf("ZMQServer start failed\n");

    return ret;
}
