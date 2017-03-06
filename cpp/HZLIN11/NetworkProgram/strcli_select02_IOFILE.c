#include "unp.h"

void str_cli(FILE *fp, int sockfd) {
    char sendline[MAXLINE];
    char recvline[MAXLINE];
    int ret;
    fd_set rset;
    int maxfdp1;
    FILE *wfp = fopen("recvfile", "w");
    fclose(wfp);
    wfp = fopen("recvfile", "a+");
    int stdineof = 0;
//    int i = 1, j = 1;
    char *ptr = NULL;

    for ( ; ; ) {
	FD_ZERO(&rset);
	if (stdineof == 0)
	    FD_SET(fileno(fp), &rset);
	FD_SET(sockfd, &rset);
	
        maxfdp1 = max(fileno(fp), sockfd) + 1;
	Select(maxfdp1, &rset, 0, 0, 0);

	if (FD_ISSET(fileno(fp), &rset)) {
	    if (NULL == fgets(sendline, MAXLINE, fp)) {
		stdineof = 1;
		shutdown(sockfd, SHUT_WR);
		printf("*********************************input file read compelete*******************************\n");
		continue;
	    }
	    Writen(sockfd, sendline, strlen(sendline));
	}
	
	if (FD_ISSET(sockfd, &rset)) {  //!!!true after recived FIN(this is select ready one of conditons). so will go to this if even readline use buffer.
again:
	    if ( (ret = readline(sockfd, recvline, MAXLINE)) < 0) {
		Printf("read error");
		fclose(wfp);
		return;
	    }
	    else if (0 == ret) {
		if (stdineof == 1) {
		    fclose(wfp);
		    printf("done\n");
		    return;
		}
		else {
		    Printf("str_cli: server terminate prematurely");
		    fclose(wfp);
		    return;
		}
	    }
	    
	    fputs(recvline, wfp);
	    
	    
//	    printf("<<<<<<<<<<<<<<readline times:i is %d\n", i);
	    if (readlinebuf_left(&ptr) > 1 ) { 
		goto again;
//		printf("<<<<<<<<<<<<<<<<<readline+bufleft: i+j is %d, the left data is :\n%s",  i+j, ptr); 
//		j++;	
	    }
//	    i++;
	}
    }

    fclose(wfp);
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
