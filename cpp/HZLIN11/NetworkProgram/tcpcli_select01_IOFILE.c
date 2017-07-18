#include "lib/unp.h"

int main(int argc, char* argv[]) {
    int i, sockfd[5];
    struct sockaddr_in servaddr;
    const char *filename = argv[2];

    if (3 != argc) {
	printf("usage: tcpcli IPaddress filename\n");
	return -1;
    }


    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    
    for (i = 0; i < 5; i++) {
	sockfd[i] = Socket(AF_INET, SOCK_STREAM, 0);
	Connect(sockfd[i], (SA *) &servaddr, sizeof servaddr);
    }

    FILE *fp = fopen(filename, "r");
    str_cli(fp, sockfd[0]);
    usleep(100000000);
    shutdown(sockfd[0], "SHUT_RDWR");
    fclose(fp);

    return 0;
}

