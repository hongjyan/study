#ifndef __TCPAPPSERVER_HPP__
#define __TCPAPPSERVER_HPP__
#include "../AppCommon.hpp"
#include "../../tcp/TcpServer.hpp"

class TcpAppServer : public AppCommon {
public:
    TcpAppServer(int argc, char* argv[]);
    ~TcpAppServer();

    void run();
private:
    TcpServer tcpServer_;
};
#endif
