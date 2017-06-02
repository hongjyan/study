#include "TcpAppServer.hpp"

TcpAppServer::TcpAppServer(int argc, char* argv[]): AppCommon(argc, argv)
{
}

TcpAppServer::~TcpAppServer() {}

void TcpAppServer::run() {
    if (localaddr_.empty() || 0==localport_) {
        printf("--localaddr and --localport should be specified as TCP server\n");
        return;
    }

    start(tcpServer_);
}
