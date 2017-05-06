#ifndef __TCPCLIENT_HPP__
#define __TCPCLIENT_HPP__
#include "TcpCnx.hpp"

class TcpClient : public TcpCnx {
public:
    TcpClient() {}
    ~TcpClient() {}

    bool start(const char *localaddr=NULL, unsigned short lport=0, const char *remoteaddr=NULL, unsigned short rport=0);
    bool stop();
};

#endif
