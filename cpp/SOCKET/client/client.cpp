#include <iostream>
#include <cstring>  //Needed for memset
#include <sys/socket.h>
#include <netdb.h>  //Needed for addrinfo, getaddrinfo(..), gai_strerror(int);
#include <stdlib.h>
#include <cstdio>
#include <arpa/inet.h>
//
#define IP_SERVER "10.68.128.132"
#define PORT_SERVER "6666"

int main()
{
    int status;
    struct addrinfo host_info;
    struct addrinfo *host_info_list, *p;

    memset(&host_info, 0, sizeof host_info);

    //won't getaddrinfo set fields below?
    host_info.ai_family = AF_UNSPEC; 
    host_info.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(IP_SERVER, PORT_SERVER, &host_info, &host_info_list);

    if (status != 0) std::cout << "getaddrinfo error" << gai_strerror(status) ;

    std::cout << "Creating a socket..." << std::endl;

    int socketfd;
    for (p=host_info_list; p!=NULL; p=p->ai_next) {
	if ((socketfd = socket(p->ai_family, p->ai_socktype, host_info.ai_protocol)) == -1) continue;
	
	printf("socketid=%d, family=%d, protocol=%d\r\n", socketfd, p->ai_family, host_info.ai_protocol);
	break;
    }
    if (p==NULL) {
	printf("creating socket failed\n");
	exit(-1);
    }

    std::cout << "connecting server..." << std::endl;
    if ((status = connect(socketfd, p->ai_addr, p->ai_addrlen)) == -1) {
	printf("Connecting failed\n");
	exit(-1);
    }

    freeaddrinfo(host_info_list);

/*
    std::cout << "sending .." << std::endl;
    char *msg = "GET / HTTP/1.1\nhost: www.baidu.com\n\n";
    int len = strlen(msg);
    ssize_t bytes_sent;
    bytes_sent = send(socketfd, msg, len, 0);
    if (bytes_sent == -1) 
    { 
	std::cout << "sending error" << std::endl; 
    }
    else
    { 
	std::cout << bytes_sent << " bytes sent." << std::endl; 
    }
*/

    std::cout << "receiving data..." << std::endl;
    char incoming_data_buffer[1000];
    ssize_t bytes_received;
    sockaddr from;
    socklen_t len = sizeof from;
    bytes_received = recv(socketfd, incoming_data_buffer, 1000, 0);
    getpeername(socketfd, &from, &len);

    char s[INET6_ADDRSTRLEN];    
    socklen_t slen = sizeof s;
    inet_ntop(AF_INET, &(((struct sockaddr_in*) &from)->sin_addr), s, slen);
    printf("len is %d, p_saddr->sin_family is %hd, p_saddr->sin_port is %hu, p_saaddr->sin_addr is %s\n",
	   len, ((struct sockaddr_in*) &from)->sin_family, ntohs(((struct sockaddr_in*) &from)->sin_port), s);

    if (bytes_received == -1) {
	std::cout << "Error happened during reveived" << std::endl;
    }
    else if (bytes_received == 0) {
	std::cout << "peer shutdown orderly" << std::endl;
    }
    else {
	std::cout << "received " << bytes_received << " bytes" << std::endl;
    }
    std::cout << incoming_data_buffer << std::endl;
    close(socketfd);
}
