#include <iostream>
#include <cstring>  //Needed for memset
#include <sys/socket.h>
#include <netdb.h>  //Needed for addrinfo, getaddrinfo(..), gai_strerror(int);
#include <stdlib.h>
#include <cstdio>
//
#define IP_SERVER "127.0.0.1"
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
    bytes_received = recv(socketfd, incoming_data_buffer, 1000, 0);
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
