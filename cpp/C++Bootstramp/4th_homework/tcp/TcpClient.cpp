#include "TcpClient.hpp"

bool TcpClient::start(const char *localaddr, unsigned short localport, const char *remoteaddr, unsigned short remoteport) {
    if (NULL==remoteaddr || 0==remoteport) {
        printf("TcpClient start failed, remote address and remote port should be specified.\n");
        return false;
    }

    bool bRet = false;

    bRet = sock_.create(SOCK_STREAM);

    if (bRet) {
        printf("TcpClient socket create succeed\n");
        bRet = sock_.setReuseAddr();
    }

    if (bRet) {
        printf("TcpClient socket setReuseAddr success\n");
        if (NULL!=localaddr && 0!=localport) {
            bRet = sock_.bind(localport, localaddr);
            if (bRet)
                printf("TcpClient socket binded succeed\n");
        }
    }

    if (bRet) {
        bRet = sock_.connect(remoteaddr, remoteport);
    }

    if (bRet) {
        printf("TcpClient socket connect succeed\n");
        bRet = startThread();
    }

    if (bRet) {
        printf("TcpClient start succeed\n");
        return bRet;
    }

    printf("TcpClient start failed, error msg is %s\n", Socket::getLastErrorMsg().c_str());
    return bRet;
}

bool TcpClient::stop() {
    bool bRet = true;

    stopThread();

    if (!sock_.close()) {
        printf("TcpClient closed data socket failed\n");
        bRet = false;
    }

#ifdef DEBUG
    printf("Close TcpClient successfully\n");
#endif

    return bRet;
}
