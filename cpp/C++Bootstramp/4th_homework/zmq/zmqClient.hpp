#ifndef __ZMQCLIENT_HPP__
#define __ZMQCLIENT_HPP__
#include "zmqCnx.hpp"

class ZMQClient : public ZMQCnx
{
public:
    ZMQClient();
    ~ZMQClient();
    bool start(const char* localaddr="", unsigned short lport=0, const char* remoteaddr="", unsigned short rport=0);
};

#endif
