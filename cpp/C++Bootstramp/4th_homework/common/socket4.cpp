#include "socket4.hpp"
#include <cstring>

Socket4::Socket4() {
}

Socket4::~Socket4() {
}

bool Socket4::create(int type, int protocol) {
    bool bRet = false;
    if (-1 == fd_) {
        if (-1 != (fd_ = ::socket(AF_INET, type, protocol))) {
            bRet = true;
        } else {
            bRet = false;
        }
    } else {
        errno = EBUSY;
        bRet = false;
    }

    return bRet;
}

bool Socket4::bind(unsigned short port, const char* addr) {
    bool bRet = false;
    struct sockaddr_in socketaddr;

    bRet = fillSockaddr(port, addr, socketaddr);

    if (bRet) {
        if (-1 != ::bind(fd_, (struct sockaddr*)&socketaddr, sizeof(socketaddr)))
            bRet = true;
        else
            bRet = false;
    }

    return bRet;
}

bool Socket4::connect(const char *addr, unsigned short port) {
    struct sockaddr_in socketaddr;

    if (!fillSockaddr(port, addr, socketaddr))
        return false;


    if (0 == ::connect(fd_, (sockaddr*)&socketaddr, sizeof(socketaddr))) {
        return true;
    } else
        return false;
}

bool Socket4::fillSockaddr(unsigned short port, const char *addr, sockaddr_in& socketaddr) {
    bool bRet = false;
    memset(&socketaddr, 0, sizeof(socketaddr));
    int r = 0;

    if (NULL == addr) {
        socketaddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
        socketaddr.sin_port = ::htons(port);
        socketaddr.sin_family = AF_INET;
        bRet = true;
    } else {
        if (0 < (r = ::inet_pton(AF_INET, addr, &socketaddr.sin_addr))) {
            socketaddr.sin_family = AF_INET;
            socketaddr.sin_port = ::htons(port);
            bRet = true;
        } else if (0 == r) {
            errno = EINVAL;
            bRet = false;
        } else {
            bRet = false;
        }
    }

    return bRet;
}

bool Socket4::getPeername(std::string &addr, unsigned short &port) {
    struct sockaddr_in socketaddr;
    socklen_t len = sizeof (socketaddr);
    bool bRet = false;

    ::bzero(&socketaddr, sizeof (socketaddr));

    if (::getpeername(fd_, (struct sockaddr *) & socketaddr, &len) != -1)
        bRet = getAddr(socketaddr, addr, port);
    else
        bRet = false;

    return bRet;
}

bool Socket4::getAddr(const struct sockaddr_in &socketaddr, std::string &addr, unsigned short &port) {
    char dst[INET_ADDRSTRLEN];
    bool bRet = false;

    if (::inet_ntop(AF_INET, &socketaddr.sin_addr, dst, sizeof (dst)) != NULL) {
        addr = dst;
        port = ::ntohs(socketaddr.sin_port);
        bRet = true;
    } else
        bRet = false;

    return bRet;
}



