#ifndef __TCPCNX_HPP__
#define __TCPCNX_HPP__
#include "../common/socket4.hpp"
#include "../common/ICnx.hpp"

#define MAX_FRAME 65535

class TcpCnx : public ICnx {
public:
    TcpCnx();
    virtual ~TcpCnx();
    int send(const void* buf, unsigned int len, int timeout);

protected:
    void recvThread();
    Socket4 sock_; //socket for data file descripter.
};

#endif
