#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__
#include "zmq.h"
#include <string>
#include <cstdio>
#include <sstream>
#include <pthread.h>
#include <thread>
#include <unistd.h>
#include <getopt.h>
#include <iostream>
#include <cstring>

#define MAXRECV 65535
typedef void (*pCallback) (const char *who, void *buf, unsigned int len);

class  Client {
public:
    Client();
    ~Client();

    void setRecvCallback(pCallback pRecvCallback);
    void run(int argc, char* argv[]);
private:
    bool start();
    void stop();
    int send(const void *buf, unsigned int len, int flag=0);
    int recv(void *buf, unsigned int len);
    void dealArgs(int argc, char* argv[]);
    std::string remoteaddr_, remoteport_;
    void *ctx_, *zmqSocket_;
    void recvDaemon();
    pCallback pRecvCallback_;
    std::thread recvThread_;
    bool bStop_;
};
#endif
