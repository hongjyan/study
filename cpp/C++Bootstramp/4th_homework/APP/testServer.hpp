#ifndef __TESTSERVER_HPP__
#define __TESTSERVER_HPP__
#include "testCommon.hpp"
#include "../tcp/TcpServer.hpp"

class TestServer : public TestCommon {
public:
    TestServer(int argc, char* argv[]);
    ~TestServer();

    void run();
private:
    TcpServer tcpServer_;
};
#endif
