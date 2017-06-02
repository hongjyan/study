#ifndef __SOCKET4_HPP__
#define __SOCKET4_HPP__

#include "socket.hpp"
#include <arpa/inet.h>

class Socket4 : public Socket
{
public:
    Socket4();
    ~Socket4();
    virtual bool create(int type, int protocol=0);
    virtual bool bind(unsigned short port, const char *addr=0);
    virtual bool connect(const char* addr, unsigned short port);
    bool getPeerName(std::string &addr, unsigned short &port);
    static bool getAddr(const struct sockaddr_in &socketaddr, std::string &addr, unsigned short &port);
    static bool getPeerName(std::string &addr, unsigned short &port, int fd);
private:
    bool fillSockaddr(unsigned short port, const char *addr, struct sockaddr_in &sockaddr);
};

#endif
