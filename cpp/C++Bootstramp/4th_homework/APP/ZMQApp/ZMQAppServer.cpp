#include "ZMQAppServer.hpp"

ZMQAppServer::ZMQAppServer(int argc, char* argv[]): AppCommon(argc, argv)
{
}

ZMQAppServer::~ZMQAppServer() {}

void ZMQAppServer::run() {
    if (localaddr_.empty() || 0==localport_) {
        printf("--localaddr and --localport should be specified as TCP server\n");
        return;
    }

    start(zmqServer_);
}
