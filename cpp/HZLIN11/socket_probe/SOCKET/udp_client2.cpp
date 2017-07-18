#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>  //perror
#include <unistd.h>   //for close
#include <string.h> // for bzero, strerror
#include <stdlib.h> // for free
#include <errno.h> //errno

#define SRVIP "127.0.0.1"
#define SRVPORT 8886
#define CLTIP "127.0.0.1"
#define CLTPORT 6666

int main(int argc, char* argv[]) {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in srvAddr, cltAddr;
    bzero(&srvAddr, sizeof srvAddr);
    bzero(&cltAddr, sizeof cltAddr);
    inet_pton(AF_INET, SRVIP, &srvAddr.sin_addr);
    srvAddr.sin_port = htons(SRVPORT);
    inet_pton(AF_INET, CLTIP, &cltAddr.sin_addr);
    cltAddr.sin_port = htons(CLTPORT);


    char sip[100],
         cip[100];
    inet_ntop(AF_INET, &srvAddr.sin_addr, sip, sizeof sip);
    inet_ntop(AF_INET, &cltAddr.sin_addr, cip, sizeof cip);
    printf("sip is %s, cip is %s\n", sip, cip);

    int yes = 1;
    if (0 != (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes))) {printf("setsockopt failed\n");}
    if (0 != (bind(fd, (sockaddr*) &cltAddr, (socklen_t) sizeof cltAddr))) { printf("bind failed\n"); return -1; }
  
    if (-1 == (connect(fd, (sockaddr*) &srvAddr, (socklen_t ) sizeof srvAddr))) { printf("connect failed\n"); return -1; }

    printf("input lines, break by CTRL+D\n");
    char* line = NULL;
    size_t len = 0;
    ssize_t read = 0;
    while ((read = getline(&line, &len, stdin)) != -1) {
        if ((sendto(fd, line, read, 0, (sockaddr*) &srvAddr, sizeof srvAddr)) != -1) continue;
        perror(strerror(errno));
        break;
    }
    //close(fd);
    shutdown(fd, SHUT_RDWR);
    if (line) free(line);
}
     
