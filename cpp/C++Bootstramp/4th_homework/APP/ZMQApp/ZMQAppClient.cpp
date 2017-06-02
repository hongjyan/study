#include "ZMQAppClient.hpp"

ZMQAppClient::ZMQAppClient(int argc, char* argv[]): AppCommon(argc, argv) {}
ZMQAppClient::~ZMQAppClient() {}

void ZMQAppClient::run() {
    if (remoteaddr_.empty() || 0==remoteport_) {
        printf("remoteaddr_ and remoteport_ should be specified to test TCP client\n");
    }

    start(zmqClient_);
}
