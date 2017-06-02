#include "TcpAppClient.hpp"

TcpAppClient::TcpAppClient(int argc, char* argv[]): AppCommon(argc, argv) {}
TcpAppClient::~TcpAppClient() {}

void TcpAppClient::run() {
    if (remoteaddr_.empty() || 0==remoteport_) {
        printf("remoteaddr_ and remoteport_ should be specified to test TCP client\n");
    }

    start(tcpClient_);
}
