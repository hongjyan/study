#include "unp.h"

int Socket(int domain, int type, int protocol) {
    int n;
    if ((n = socket(domain, type, protocol)) < 0) {
	Printf("create socket failed");
	exit(1);
    }
    return n;
}


int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
    int acceptsock;
    if ((acceptsock = accept(sockfd, addr, addrlen)) < 0) {
	Printf("Accept error");
	exit(1);
    }
    return acceptsock;
}

int Listen(int sockfd, int backlog) {
    char *ptr;
    if (NULL != (ptr = getenv("LISTENQ"))) 
	backlog = atoi(ptr);

    if (listen(sockfd, backlog) < 0 ) {
	Printf("Listen failed");
	exit(1);
    }

    return 0;
}

int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    if (bind(sockfd, addr, addrlen) < 0) {
	Printf("Bind failed");
	exit(1);
    }
    
    return 0;
}

int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    if (connect(sockfd, addr, addrlen) != 0) {
	Printf("connect failed");
	exit(1);
    }

    return 0;
}


int Select(int sockfd, fd_set *rset, fd_set *wset, fd_set *eset, struct timeval *timeout) {
    int ready;
    if ( (ready = select(sockfd, rset, wset, eset, timeout)) < 0) {
	Printf("select failed");
	exit(1);
    }
    
    return ready;
}

