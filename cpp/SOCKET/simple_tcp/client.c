#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <errno.h>

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);

/*
    connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
    printf("writable, Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buffer);

    close(sockfd);
    return 0;
*/


    int connErr;
    int val;
    if (-1 == (val=fcntl(sockfd, F_GETFL, 0))) {
        printf("F_GETFL for sockfd failed\n");
        return 0;
    }
    if (!(val&FNDELAY)) {
        val |= FNDELAY;
        fcntl(sockfd, F_SETFL, val);
    }
    connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));

    do { 
        fd_set write_fds, except_fds;
        FD_ZERO(&write_fds);
        FD_ZERO(&except_fds);
        FD_SET(sockfd, &write_fds);
        FD_SET(sockfd, &except_fds);
        struct timeval tv;
        tv.tv_sec = 60;
        tv.tv_usec = 0;
        if (1 == select(sockfd+1, NULL, &write_fds, &except_fds, &tv)) { 
            if (FD_ISSET(sockfd, &except_fds)) {
                printf("except happened on sockfd");
                return 1;
            }
        }
        socklen_t len = sizeof(connErr);
        getsockopt(sockfd, SOL_SOCKET, SO_ERROR, (char *)&connErr, &len);
    } while (EINPROGRESS == connErr);
 
    if (connErr) {
        printf("nonblocking connect failed by %s\n", strerror(connErr));
        return 1;
    }

    
    printf("writable, Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    n = write(sockfd,buffer,strlen(buffer));
    if (0 > n) error("write failed\n");
    

    bzero(buffer,256);
    do {
        n = read(sockfd,buffer,255);
        fd_set read_fds, except_fds;
        FD_ZERO(&read_fds);
        FD_ZERO(&except_fds);
        FD_SET(sockfd, &read_fds);
        FD_SET(sockfd, &except_fds);
        struct timeval tv; 
        tv.tv_sec = 60; 
        tv.tv_usec = 0;
        if (1 == select(sockfd+1, &read_fds, NULL, &except_fds, &tv)) { 
            if (FD_ISSET(sockfd, &except_fds)) {
                printf("except happened on sockfd");
                return 1;
            }   
        }
    } while (-1 == n && EAGAIN == errno);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buffer);

    close(sockfd);
    return 0;
}
