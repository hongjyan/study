#ifndef __TCPAPPCLIENT_HPP__
#define __TCPAPPCLIENT_HPP__
#include "../AppCommon.hpp"
#include "../../tcp/TcpClient.hpp"

class TcpAppClient : public AppCommon {
public:
    TcpAppClient(int argc, char* argv[]);
    ~TcpAppClient();
    void run();
private:
    TcpClient tcpClient_;
};
#endif
