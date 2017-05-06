#include "socket.hpp"
#include <cstring>
#include <cstdio>


Socket::Socket() {
    fd_ = -1;
}

Socket::~Socket() {
    close();
}

bool Socket::listen(int backlog) {
    if (-1  == ::listen(fd_, backlog))
        return false;
    return true;
}

bool Socket::close() {
    bool bRet = false;
    if (-1 != fd_) {
        if (0 == ::close(fd_)) {
            bRet = true;
            fd_ = -1;
        }

        bRet = false;
    }

    return bRet;
}

bool Socket::attach(int fd) {
    if (-1 == fd_) {
        fd_ = fd;
        return true;
    } else {
        errno = EPERM;
        return false;
    }
}

int Socket::detach() {
    int s = fd_;
    fd_ = -1;
    return s;
}

bool Socket::accept(Socket& sock, sockaddr* rsa, socklen_t* rsalen) {
    int s;
    bool ret;
    if (-1 != (s=::accept(fd_, rsa, rsalen))) {
        ret = sock.attach(s);

        if (!ret) {
            ::close(s);
        }
    } else {
        ret = false;
    }

    return ret;
}

int Socket::sendn(const void* buf, unsigned int len, int timeout) {
    int now = 0;
    unsigned int sent= 0;
    struct timeval time;
    time.tv_sec = timeout/1000;
    time.tv_usec = timeout%1000*1000;
    char* cbuf = (char*)buf;

    fd_set wset;
    FD_ZERO(&wset);
    FD_SET(fd_, &wset);
    int nready = 0;

    while (sent < len) {
        nready = ::select(fd_+1, NULL, &wset, NULL, &time);
        if (nready > 0) {
            now = ::send(fd_, cbuf+sent, len-sent, 0);
             if (0 > now)
                 printf("Sending failed, try till timeout\n");
             else
                 sent += now;
        } else if (nready == 0) {
            printf("Send timeout\n");
            break;
        }
        else {
            printf("Send:select failed\n");
            break;
        }
    }

    return sent;
}

int Socket::recvn(void* buf, unsigned int len, int timeout) {
    if (NULL == buf)
        return -1;

    char *cbuf = (char*)buf;
    int now = 0;
    unsigned int recvd = 0;
    struct timeval time;
    time.tv_sec = timeout/1000;
    time.tv_usec = timeout%1000*1000;

    fd_set rset;
    FD_ZERO(&rset);
    FD_SET(fd_, &rset);
    int nready;

    while (recvd < len) {
        nready = ::select(fd_+1, &rset, NULL, NULL, &time);
        if (0 < nready) { //FD_ISSET(fd_, &rset) again? A: no need
            now = ::recv(fd_, cbuf+recvd, len-recvd, 0);
            if (0 > now)
                recvd += now;
            else
                printf("Recving failed, try next till timeout\n");
        } else if (0 == nready) {
            printf("recv timeout\n");
            break;
        } else {
            printf("Recv:select failed\n");
            break;
        }
    }

    return recvd;
}

int Socket::recv(void *buf, unsigned int len) {
    if (NULL == buf) {
        printf("buf is NULL pointer\n");
        return -1;
    }

    int recvd;
    char *cbuf = (char*)buf;

    fd_set rset;
    FD_ZERO(&rset);
    FD_SET(fd_, &rset);
    int nready = select(fd_+1, &rset, NULL, NULL, NULL);
    switch (nready) {
    case 0:
        printf("Timeout during recv-select\n");
        return -1;
    case -1:
        printf("Error happened during recv-select, reason:%s\n", getLastErrorMsg().c_str());
        return -1;
    default:
        recvd = ::recv(fd_, cbuf, len, 0);
    }

    if (recvd <= 0) {
        printf("Error happened during recv, reason:%s\n", getLastErrorMsg().c_str());
        return -1;
    }

    return recvd;
}

bool Socket::setNonBlockMode(bool enable) {
    bool bRet = false;
    int val;

    val = ::fcntl(fd_, F_GETFL, 0);

    if (val >= 0) {
        if (enable)
            bRet = (-1 != ::fcntl(fd_, F_SETFL, val | O_NONBLOCK)) ? true : false;
        else
            bRet = (-1 != ::fcntl(fd_, F_SETFL, val & ~O_NONBLOCK)) ? true : false;
    }

    return bRet;
}

int Socket::getLastError() {
    return errno;
}

std::string Socket::getLastErrorMsg() {
    return std::string(::strerror(errno));
}


bool Socket::setReuseAddr(bool enable) {
    bool bRet = false;
    int optval;

    optval = enable ? 1 : 0;

    if (::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, (const void *) & optval, sizeof (optval)) != -1)
        bRet = true;
    else
        bRet = false;

    return bRet;
}



