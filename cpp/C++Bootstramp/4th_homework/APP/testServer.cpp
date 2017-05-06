#include "testServer.hpp"

TestServer::TestServer(int argc, char* argv[]): TestCommon(argc, argv)
{
}

TestServer::~TestServer() {}

void TestServer::run() {
    if (localaddr_.empty() || 0==localport_) {
        printf("--localaddr and --localport should be specified as TCP server\n");
        return;
    }

    start(tcpServer_);
}
