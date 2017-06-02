#ifndef __TESTCLIENT_HPP__
#define __TESTCLIENT_HPP__
#include "testCommon.hpp"
#include "../tcp/TcpClient.hpp"

class TestClient : public TestCommon {
public:
    TestClient(int argc, char* argv[]);
    ~TestClient();
    void run();
private:
    TcpClient tcpClient_;
};
#endif
