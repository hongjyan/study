#include "lib/unp.h"

int main(int argc, char* argv[]) {
    int listenfd, connfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t clilen;
    pid_t childpid;
    int yes = 1;

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    Bind(listenfd, (SA *) &servaddr, sizeof servaddr);
    Listen(listenfd, LISTENQ);

    signal(SIGCHLD, sig_chld);
    
    for( ; ; ) 
    {
//move lines below after Accept. same effect.
	fd_set rset;
	FD_ZERO(&rset);
	FD_SET(listenfd, &rset);
	int ready;
	if ( (ready = select(listenfd+1, &rset, 0, 0, 0)) <= 0) {
	    Printf("select failed");
	    exit(1);
	}	
	printf("select success\n");
//

	clilen = sizeof(cliaddr);
	connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);
    

	if ((childpid = fork()) == 0) {
	    close(listenfd);
	    str_echo(connfd);
	    close(connfd);
	    exit(0);
	}

	close(connfd);
    }

    return 0;
	     
}

    
