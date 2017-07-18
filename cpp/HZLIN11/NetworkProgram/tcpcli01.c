#include "lib/unp.h"

int main(int argc, char* argv[]) {
    int sockfd;
    struct sockaddr_in servaddr;

    if (2 != argc) {
	printf("usage: tcpcli IPaddress\n");
	return -1;
    }

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    

    Connect(sockfd, (SA *) &servaddr, sizeof servaddr);

    str_cli(stdin, sockfd);

    return 0;
}

