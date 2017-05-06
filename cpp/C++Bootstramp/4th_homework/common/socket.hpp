#ifndef __SOCKET_HPP__
#define __SOCKET_HPP_
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>

class Socket {
public:
    Socket();
    virtual ~Socket();
    virtual bool create(int type, int protocol=0) = 0;
    virtual bool bind(unsigned short port, const char *addr=NULL) = 0;
    bool listen(int backlog);
    virtual bool connect(const char *addr, unsigned short port)=0;
    bool accept(Socket &sock, sockaddr* rsa=NULL, socklen_t* rsalen=NULL);
    int sendn(const void *buf, unsigned int len, int timeout);
    int recvn(void *buf, unsigned int len, int timeout);
    int recv(void *buf, unsigned int len, int timeout);
    int recv(void *buf, unsigned int len);
    bool close();

    bool attach(int fd);
    int detach();
    bool setNonBlockMode(bool enable = true);
    bool setReuseAddr(bool enable=true);
    static int getLastError();
    static std::string getLastErrorMsg();
protected:
    int fd_;
};

#endif
