#include "TcpCnx.hpp"

TcpCnx::TcpCnx() 
{
}

TcpCnx::~TcpCnx() 
{
}


void TcpCnx::recvThread() {
    bool bStop = false;
    char msgbuf[MAX_FRAME] = {0};
    int recvd = 0;

    while (!bStop && !stopThread_) {
        recvd = sock_.recv(msgbuf, MAX_FRAME);
        if (-1 == recvd) {
            if (ECONNRESET == errno) {
                printf("TcpCnx::recvThread---connection reset by peer. %s:%d\n", __FILE__, __LINE__);
                bStop = true;
            } else {
                printf("TcpCnx::recvThread---recv failed, reason is %s\n", Socket::getLastErrorMsg().c_str());
                return;
            }
        }

        std::string raddr;
        unsigned short rport=0;
        if (nullptr != callback_)
            sock_.getPeerName(raddr, rport);
            callback_(('\n'+raddr+':'+std::to_string(rport)).c_str(), msgbuf, recvd);
    }
}


int TcpCnx::send(const void* buf, unsigned int len, int timeout) {
    return sock_.sendn(buf, len, timeout);
}




