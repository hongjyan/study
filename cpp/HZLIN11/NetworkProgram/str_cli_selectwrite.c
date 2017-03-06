#include "unp.h"

void str_cli(FILE *fp, int sockfd) {
    char sendline[MAXLINE];
    char recvline[MAXLINE];
    int ret;
    printf("2\n");
    while (NULL != fgets(sendline, MAXLINE, fp)) {
        int ready;
        fd_set wset;
        FD_ZERO(&wset);
        FD_SET(sockfd, &wset);
	printf("3\n");
        if ( (ready = select(sockfd+1, 0, &wset, 0, 0)) <= 0)
            Printf("select error");

	printf("4\n");

	if (0 >= Writen(sockfd, sendline, strlen(sendline))) {
	    Printf("write error");
	    return;
	}
	
	printf("5\n");
	
	if ( (ret = readline(sockfd, recvline, MAXLINE)) < 0) {
	    Printf("read error");
	    return;
	}
	else if (0 == ret) {
	    Printf("str_cli: server terminate prematurely");
	    return;
	}
	
	fputs(recvline, stdout);
	close(sockfd); //add to check 6.3.1 writeset condition b
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
