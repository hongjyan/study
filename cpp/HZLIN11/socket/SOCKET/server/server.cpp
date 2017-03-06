#include <cstring> //memset, strlen
#include <stdlib.h> //exit
#include <stdio.h> //printf
#include <sys/socket.h>
#include <netdb.h> //addr_info, getaddrinfo, freeaddrinfo, gai_strerror
#include <arpa/inet.h> //inet_top
#include <unistd.h> //close(socketfd)
#include <errno.h>
//#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#define backlog 5
#define PORT "6666"
#define IP "10.68.128.132"


void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
    	return &(((struct sockaddr_in *)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

void sigchld_handler(int s)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}


int main() {
    int sockfd, new_fd, rv;
    struct addrinfo hints, *serverinfo_list, *p;
    struct sockaddr clientaddr;
    fd_set read_fds;
    struct timeval timeout;
    int yes=1;
    int nready;
    
    memset(&hints, 0, sizeof(addrinfo));
    //if ((rv = getaddrinfo(NULL, PORT, &serverinfo_list)) != 0) {
    if ((rv = getaddrinfo(IP, PORT, &hints, &serverinfo_list)) != 0) {
	printf("get_addr_info failed:%s\n", gai_strerror(rv));
    };
    

    printf("creatig socket\n");
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    
    for (p=serverinfo_list; p!=NULL;  p=p->ai_next) {
	if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) continue;
	
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
	    printf("set socket option failed\n");
	    exit(-1);
	}

	if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
	    printf("binding failed\n");
	    close(sockfd);
	    continue;
	}
	
	break;
    }

    if (p==NULL) {
	printf("no avaliable service:ip+port for creating socket or creating failed");
	exit(-1);
    }


    freeaddrinfo(serverinfo_list);

    printf("listening socket\n");
    if ((listen(sockfd, 5)) == -1) {
	printf("listening on socket failed\n");
	exit(-1);
    }

/*    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
	printf("sigaction failed\n");
	exit(-1);
    }
*/
    printf("selecting...........\n");
    FD_SET(sockfd, &read_fds);
    timeout.tv_sec = 60;
    timeout.tv_usec = 0;
    switch (nready = select(sockfd+1, &read_fds, NULL, NULL, &timeout)) {
	case -1 :
	    printf("Error happened during select\n");
	    exit(-1);
	    break;
	case 0 :
	    printf("Time experied\n");
	    exit(-1);
	    break;
	default :
	    ;
    }

    printf("accepting socket\n");
    socklen_t sin_size = sizeof clientaddr;
    if ((new_fd = accept(sockfd, &clientaddr, &sin_size)) == -1) {
	printf("accept failed\n");
	exit(-1);
    }
    else {
	printf("new socket:%d creted\n", new_fd);
	close(sockfd);
    }
    char s[INET6_ADDRSTRLEN];
    inet_ntop(clientaddr.sa_family, get_in_addr((struct sockaddr *) &clientaddr), s, sizeof s);
    printf("got connection from %s\n", s);

    printf("sending data\n");
    const char *data = "Hello World!";
    const int bytes_data = strlen(data) + 1;
    int bytes_sent = send(new_fd, data, bytes_data, 0);
    if (bytes_sent == -1) { 
	printf("sending data failed\n");
	printf("errno is %d, %s\n", errno, strerror(errno));
	exit(-1);
    }
    else if (bytes_sent == bytes_data) {
	printf("%d bytes(compelete) were sent\n", bytes_sent);
    }
    else {
	printf("%d bytes(partial) were sent\n", bytes_sent);
    }
}
