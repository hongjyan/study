#include "unp.h"

void str_echo(int sockfd) {
    ssize_t rnum, wnum;
    char buf[MAXLINE]; 

    while (0 < (rnum = recv(sockfd, buf, MAXLINE, 0))) { //it is OK to use recv instead of readline 
	wnum = Writen(sockfd, buf, rnum);
        if (0 > wnum) 
	    Printf("write error");
        else if (wnum != rnum) 
	    Printf("write not compelete");
	else 
	    printf("write %zd bytes compeletely\n", wnum);
    }
	

    if (0 > rnum) Printf("read error");
}
    
