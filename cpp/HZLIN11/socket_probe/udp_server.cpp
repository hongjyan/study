#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>  //for strerror(), bzero
#include <errno.h>
#define SRVIP "127.0.0.1"
#define SRVPORT 58040
#define CLTIP "127.0.0.1"
#define CLTPORT 6666

int main(int argc, char* argv[]) {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in srvAddr, cltAddr;
    bzero(&srvAddr, sizeof srvAddr);
    bzero(&cltAddr, sizeof cltAddr);
    inet_pton(AF_INET, SRVIP, &srvAddr.sin_addr);
    srvAddr.sin_port = htons(SRVPORT);
    srvAddr.sin_family = AF_INET;
    inet_pton(AF_INET, CLTIP, &cltAddr.sin_addr);
    cltAddr.sin_port = htons(CLTPORT);
    cltAddr.sin_family = AF_INET;

    char ip[100];
    inet_ntop(AF_INET, &srvAddr.sin_addr, ip, sizeof(ip));
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
     
