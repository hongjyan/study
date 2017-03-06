#include "unp.h"

void str_cli(FILE *fp, int sockfd) {
    char sendline[MAXLINE];
    char recvline[MAXLINE];
    int ret;
    int ready;
    fd_set wset;
    int sendbufsize = 4096;
    
    while (NULL != fgets(sendline, MAXLINE, fp)) {
	FD_ZERO(&wset);
	FD_SET(sockfd, &wset);
        
        if (setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &sendbufsize, sizeof sendbufsize) != 0) {
            Printf("set SO_SNDBUF failed\n");
            return;
        }


	if ( (ready = select(sockfd+1, 0, &wset, 0, 0)) <= 0) {
	    Printf("select failed");
	    return;
	}
	
	if (0 > Writen(sockfd, sendline, strlen(sendline))) {
	    Printf("write error");
	    return;
	}

	if ( (ret = readline(sockfd, recvline, MAXLINE)) < 0) {
	    Printf("read error");
	    return;
	}
	else if (0 == ret) {
	    Printf("str_cli: server terminate prematurely");
	    return;
	}
	
	fputs(recvline, stdout);
    }
}


//Version to generate SIGPIPE, corporate with tcpcli_sigpipe.c
/*
void str_cli(FILE *fp, int sockfd) {
    char sendline[MAXLINE];
    char recvline[MAXLINE];
    int ret;
    while (NULL != fgets(sendline, MAXLINE, fp)) {
	if (0 > Writen(sockfd, sendline, 1)) {
	    Printf("write error");
	    return;
	}
	sleep(1); //sleep 1s to let RST in socket buffer.
	printf("enter while\n");
	if (0 > Writen(sockfd, sendline+1, strlen(sendline)-1)) {
	    Printf("write error");
	    return;
	}

	if ( (ret = readline(sockfd, recvline, MAXLINE)) < 0) {
	    Printf("read error");
	    return;
	}
	else if (0 == ret) {
	    Printf("str_cli: server terminate prematurely");
	    return;
	}
	
	fputs(recvline, stdout);
    }
}
*/
