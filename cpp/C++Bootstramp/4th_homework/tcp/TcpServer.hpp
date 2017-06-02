#ifndef __TCPSERVER_HPP__
#define __TCPSERVER_HPP__
#include "TcpCnx.hpp"

class TcpServer : public TcpCnx {
public:
    TcpServer() {}
    ~TcpServer() {}

    bool start(const char *localaddr=NULL, unsigned short lport=0, const char *remoteaddr=NULL, unsigned short rport=0);
    bool stop();
private:
    Socket4 listenningSock_;
};
#endif
