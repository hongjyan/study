#ifndef __ZMQSERVER_HPP__
#define __ZMQSERVER_HPP__
#include "zmqCnx.hpp"

class ZMQServer : public ZMQCnx {
public:
    ZMQServer();
    ~ZMQServer();
    bool start(const char* localaddr="", unsigned short lport=0, const char* remoteaddr="", unsigned short rport=0);
};

#endif
