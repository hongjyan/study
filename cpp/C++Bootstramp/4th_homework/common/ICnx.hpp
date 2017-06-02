#ifndef __CNX_HPP__
#define __CNX_HPP__

#include <pthread.h>
#include <unistd.h>
#include <cstring>
#include <errno.h>
#include <cstdio>

typedef void (*Callback) (const char* who, void *buf, unsigned int len);

class ICnx {
public:
    ICnx();
    virtual ~ICnx();
    virtual bool start(const char* localaddr="", unsigned short lport=0, const char* remoteaddr="", unsigned short rport=0)=0;
    virtual bool stop()=0;
    bool startThread();
    void stopThread();
    virtual int send(const void* buf, unsigned int len, int timeout)=0;
    void setCallback(Callback callback);

protected:
    static void* recvThread(void* arg);
    virtual void recvThread()=0;
    bool stopThread_;
    pthread_t tidDataThread;
    Callback callback_;
};

#endif
