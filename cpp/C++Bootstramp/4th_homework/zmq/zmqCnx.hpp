#ifndef __ZMQCNX_HPP__
#define __ZMQCNX_HPP__
#include <zmq.h>
#include "../common/ICnx.hpp"
#include <string>
#include "../common/socket4.hpp"

#define MAX_FRAME 65535

class ZMQCnx : public ICnx
{
public:
    ZMQCnx();
    virtual ~ZMQCnx();
    bool stop();
    int send(const void* buf, unsigned int len, int timeout);
    void recvThread();
protected:
    void* zmqContext_;
    void* zmqSocket_;
};

#endif
