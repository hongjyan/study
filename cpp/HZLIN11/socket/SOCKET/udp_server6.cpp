#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>  //for strerror(), bzero
#include <errno.h>
#define SRVIP "::01"
#define SRVPORT 8888
#define CLTIP "::01"
#define CLTPORT 6666

int main(int argc, char* argv[]) {
    int fd = socket(AF_INET6, SOCK_DGRAM, 0);

    sockaddr_in6 srvAddr, cltAddr;
    bzero(&srvAddr, sizeof srvAddr);
    bzero(&cltAddr, sizeof cltAddr);
    inet_pton(AF_INET6, argv[1], &srvAddr.sin6_addr);
    srvAddr.sin6_port = htons(SRVPORT);
    inet_pton(AF_INET6, CLTIP, &cltAddr.sin6_addr);
    cltAddr.sin6_port = htons(CLTPORT);

    char ip[100];
    inet_ntop(AF_INET6, &srvAddr.sin6_addr, ip, sizeof(ip));
    printf("ip is %s\n", ip); 

    if ( 0 != (bind(fd, (sockaddr*) &srvAddr, sizeof srvAddr))) { printf("bind failed\n"); perror(strerror(errno)); return -1;}
    char buf[1024];
    memset(buf, 0, sizeof buf);
    socklen_t len;
    while (1) {
        if ((recvfrom(fd, buf, 1024, 0, (sockaddr*) &cltAddr, &len)) <= 0) break;
        printf("%s\n", buf);
    }
}
     
