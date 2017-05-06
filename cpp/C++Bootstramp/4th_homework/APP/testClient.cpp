#include "testClient.hpp"

TestClient::TestClient(int argc, char* argv[]): TestCommon(argc, argv) {}
TestClient::~TestClient() {}

void TestClient::run() {
    if (remoteaddr_.empty() || 0==remoteport_) {
        printf("remoteaddr_ and remoteport_ should be specified to test TCP client\n");
    }

    start(tcpClient_);
}
