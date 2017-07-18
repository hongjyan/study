#include "unp.h"

void str_echo(int sockfd) {
    ssize_t rnum, wnum;
    char buf[MAXLINE]; 
    fd_set rset;
    int rready;
    
    for ( ; ; ) {
	//select return 1 even sockfd close by client, and readline return 0
	FD_ZERO(&rset);
	FD_SET(sockfd, &rset);
	if ( (rready = select(sockfd+1, &rset, 0, 0, 0)) == -1) {
	    Printf("select error\n");
	    return;
	}
	else if (rready == 0) {
	    printf("peer shutdown\n");
	    return;
	}

	if (!FD_ISSET(sockfd, &rset)) {
	    printf("select return, but no desired socket ready\n");
	    return;
	}
    
	rnum = readline(sockfd, buf, MAXLINE);
	wnum = Writen(sockfd, buf, rnum);
        if (0 > wnum) 
	    Printf("write error");
        else if (wnum != rnum) 
	    Printf("write not compelete");
	else
	    printf("write %zd bytes compeletely\n", wnum);
    }
}
    
