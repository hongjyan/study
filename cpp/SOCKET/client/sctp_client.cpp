#include <iostream>
#include <cstring>  //memset()
#include <sys/socket.h>
#include <netdb.h>  //addrinfo, getaddrinfo(..), gai_strerror(int);
#include <stdlib.h>
#include <cstdio>
#include <stdio.h> //FILE, fopen(), fclose()
#include <algorithm> //std::copy()
#include <errno.h>
//
#define IP_SERVER "10.69.73.78"
#define PORT_SERVER "6666"

long getFileLength(FILE *file) {
    long lCurPos, lEndPos;
    fseek(file, 0, 0);
    lCurPos = ftell(file);
    fseek(file, 0, 2);
    lEndPos = ftell(file);
    rewind(file);
    return lEndPos-lCurPos;
}

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


    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    setsockopt(socketfd, SOL_SOCKET, SO_RCVTIMEO, (char*) &timeout, sizeof timeout); 

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

    long dataLen = 0;
    int longLen = sizeof dataLen;
    ssize_t bytes_len = recv(socketfd, &dataLen, longLen, 0);
    if ( 0 == bytes_len) { printf("recieve 0 bytes\n");}
    if ( -1 == bytes_len) { printf("recv failed, the errno is %d, the error msg is %s\n", errno, strerror(errno));}
    if (bytes_len != longLen) {
        printf("receive file length failed\n");
        exit(-1);
    }
    printf("receiving file length %ld\n", dataLen);

    std::cout << "receiving data..." << std::endl;
    char incoming_data_buffer[dataLen];
    long bytes_received = -2, total_bytes_received = 0;
    long restDataLen = dataLen;
    char *incoming_data_pos = incoming_data_buffer;
    while (bytes_received != 0 && bytes_received != -1 && total_bytes_received < dataLen) {
     	bytes_received = recv(socketfd, 
                              incoming_data_pos, 
                              restDataLen, 
                              0);
        total_bytes_received += bytes_received;
        restDataLen -= bytes_received;
        incoming_data_pos += bytes_received;
        printf("%ld bytes received\n", bytes_received);
    }
    if (bytes_received == -1) {
	std::cout << "Error happened during reveived" << std::endl;
    }
    else if (total_bytes_received == 0) {
	std::cout << "peer shutdown orderly" << std::endl;
    }
    else if (total_bytes_received == dataLen) {
	printf("%ld bytes received compeletely\n", total_bytes_received);
    }
    else if (total_bytes_received > dataLen) {
        printf("%ld bytes received, exceed expected amount!!!\n", total_bytes_received);
    } 
    else {
        printf("%ld bytes received partially\n", total_bytes_received);
    }
//    std::cout << incoming_data_buffer << std::endl;

    std::cout << "sending data" << std::endl;
    FILE *pFile = NULL;
    pFile = fopen("client.cpp", "r");
    if (pFile == NULL) {
        printf("opening file failed\n");
        exit(-1);
    }
 
    const long sendFileLen = getFileLength(pFile);
    ssize_t long_len = sizeof sendFileLen;
    const long sendDataLen = long_len + sendFileLen;
    char send_data[sendDataLen];
    std::copy(static_cast<const char*>(static_cast<const void*>(&sendFileLen)),
              static_cast<const char*>(static_cast<const void*>(&sendFileLen)) + long_len,
              send_data);
    fread(send_data+long_len, sendFileLen, 1, pFile);
    fclose(pFile);

    long bytes_sent = send(socketfd, send_data, sendDataLen, 0);
    if (bytes_sent == -1) {
        printf("sending data failed\n");
        exit(-1);
    }
    else if (bytes_sent == sendDataLen) {
        printf("%ld bytes sent compeletely\n", bytes_sent);
    }
    else {
        printf("%ld bytes send partially\n", bytes_sent);
    }

    close(socketfd);
}
