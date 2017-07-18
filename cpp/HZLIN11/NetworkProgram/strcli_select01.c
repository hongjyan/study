#include "unp.h"

void str_cli(FILE *fp, int sockfd) {
    char sendline[MAXLINE];
    char recvline[MAXLINE];
    int ret;
    fd_set rset;
    int maxfdp1 = max(fileno(fp), sockfd) + 1;

    for ( ; ; ) {
	FD_ZERO(&rset);
	FD_SET(fileno(fp), &rset);
	FD_SET(sockfd, &rset);
	
	Select(maxfdp1, &rset, 0, 0, 0);

	if (FD_ISSET(fileno(fp), &rset)) {
	    if (NULL == fgets(sendline, MAXLINE, fp)) 
		return;
	    Writen(sockfd, sendline, strlen(sendline));
	}

	if (FD_ISSET(sockfd, &rset)) {
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
