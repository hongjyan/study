#include "zmqCnx.hpp"
#include <cstdio>

ZMQCnx::ZMQCnx()
{
    zmqContext_ = zmq_ctx_new();
}

ZMQCnx::~ZMQCnx()
{
    zmq_ctx_destroy(zmqContext_);
}

bool ZMQCnx::stop()
{
    stopThread();
    return true;
}

int ZMQCnx::send(const void* buf, unsigned int len, int timeout)
{
    return zmq_send(zmqSocket_, buf, len, 0);
}

void ZMQCnx::recvThread()
{
    bool bStop = false;
    char msgbuf[MAX_FRAME] = {0};
    int recvd = 0;

    while (!bStop && !stopThread_) {
        recvd = zmq_recv(zmqSocket_, msgbuf, MAX_FRAME, 0); //maybe program hang here even stopThread_ = true. need signal to interupt block here
        if (-1 == recvd) {
            printf("ZMQCnx::recvThread---recv failed, reason is %s\n", strerror(errno));
            return;
        }

        if (nullptr != callback_) {
            int fd = -1;
            size_t fd_size = sizeof(fd);
            if (-1 == zmq_getsockopt(zmqSocket_, ZMQ_FD, &fd, &fd_size)) {
                callback_("unknownaddr", msgbuf, recvd);
            } else {
                std::string raddr;
                unsigned short rport=0;
                if (Socket4::getPeerName(raddr, rport, fd))
                    callback_(('\n'+raddr+':'+std::to_string(rport)).c_str(), msgbuf, recvd);
                else
                    callback_("unknown addr", msgbuf, recvd);
            }
        }
    }
}
