#ifndef __TCPCNX_HPP__
#define __TCPCNX_HPP__
#include <pthread.h>
#include "../common/socket4.hpp"

#define MAX_FRAME 65535
typedef void (*Callback) (const char* who, void *buf, unsigned int len);

class TcpCnx {
public:
    TcpCnx();
    virtual ~TcpCnx();
    virtual bool start(const char* localaddr="", unsigned short lport=0, const char* remoteaddr="", unsigned short rport=0)=0;
    virtual bool stop()=0;
    bool startThread();
    void stopThread();
    int send(const void* buf, unsigned int len, int timeout);
    void setCallback(Callback callback);

protected:
    static void* recvThread(void* arg);
    void recvThread();
    Socket4 sock_; //socket for data file descripter.
    bool stopThread_;
    pthread_t tidDataThread;
    Callback callback_;
};

#endif
