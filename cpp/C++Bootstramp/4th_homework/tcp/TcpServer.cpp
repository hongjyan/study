#include "TcpServer.hpp"
#include <unistd.h>
#include <cstring>

bool TcpServer::start(const char *localaddr, unsigned short localport, const char *remoteaddr, unsigned short remoteport) {
    bool bRet = false;

    if (NULL==localaddr || 0==localport) {
        printf("localaddr and localport needs to be specified for TcpServer\n");
        return bRet;
    }

    bRet = listenningSock_.create(SOCK_STREAM);

    if (bRet) {
        printf("TcpServer socket created succeed\n");
        bRet = listenningSock_.setReuseAddr();
    }

    if (bRet) {
        printf("TcpServer socket set address reuse-able  succeed\n");
        bRet = listenningSock_.bind(localport, localaddr);
    }

    if (bRet) {
        printf("TcpServer socket bind succeed\n");
        bRet = listenningSock_.listen(5);
    }
    if (bRet) {
        printf("TcpServer socket listen succeed\n");
        bRet = listenningSock_.accept(sock_);

        if (bRet) {
            printf("TcpServer socket accept succeed\n");
            if ( (0!=strcmp("", remoteaddr)) && 0!=remoteport) {
                std::string raddr;
                unsigned short rport;
                sock_.getPeerName(raddr, rport);
                if (rport!=remoteport || strcmp(raddr.c_str(), remoteaddr))
                    bRet = false;
            }
        }
    }


    if (bRet) {
        bRet = startThread();
    }

    if (bRet) {
        printf("TcpServer start succeed\n");
        return bRet;
    }

    printf("TcpServer start failed, error msg is %s\n", Socket::getLastErrorMsg().c_str());
    return bRet;
}

bool TcpServer::stop() {
    bool bRet = true;

    stopThread();

    if (!sock_.close()) {
        printf("TcpServer closed data socket failed\n");
        bRet = false;
    }

    if (!listenningSock_.close()) {
        printf("TcpServer closed listenning socket failed\n");
        bRet = false;
    }

    return bRet;
}
