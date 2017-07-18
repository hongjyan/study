#include "unp.h"
#include "sum.h"

void str_cli(FILE *fp, int sockfd) {
    struct args params;
    struct result sum;
    int ret;

    for ( ; ; ) {
	if ( 2 != scanf("%ld %ld", &params.arg1, &params.arg2)) {
	    printf("invalid input\n");
	    return;
	}

	if (0 > Writen(sockfd, &params, sizeof(params))) {
	    Printf("write error");
	    return;
	}

	if ( (ret = readn(sockfd, &sum, sizeof(sum))) < 0) {
	    Printf("read error");
	    return;
	}
	else if (0 == ret) {
	    Printf("str_cli: server terminate prematurely");
	    return;
	}
	
	printf("%ld\n", sum.sum);
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
