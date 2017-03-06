#include "lib/unp.h"

int main(int argc, char* argv[]) {
    int listenfd, connfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t clilen;
    pid_t childpid;
    int yes = 1;


//try to create a new socket binding on the same port but not the same address.
   struct sockaddr_in servaddr2;
   int listenfd2;
   servaddr2.sin_family = AF_INET;
   servaddr2.sin_port = htons(SERV_PORT);
   inet_pton(AF_INET, "10.159.215.14", &servaddr2.sin_addr);
   listenfd2 = Socket(AF_INET, SOCK_STREAM, 0);
   setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
   Bind(listenfd2, (SA *) &servaddr2, sizeof servaddr2);
   Listen(listenfd2, LISTENQ);
  
  
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

    
