#include <sstream>
#include <stdio.h> //printf, FILE, fopen()
#include <sys/socket.h>
#include <netdb.h> //addr_info, getaddrinfo, freeaddrinfo, gai_strerror
#include <stdlib.h> //exit(), atol()
#include <errno.h>
#include <arpa/inet.h> //inet_top
#include <fcntl.h> //fcntl()
typedef unsigned char BYTE;


void *get_in_addr(struct sockaddr *sa);

int get_in_port(struct sockaddr *sa);

long getFileLength(FILE *file);

bool isblocking(int fd);

template <class T>
const char* toString(T& t) {
    std::stringstream ss;
    ss << t;
    return ss.str().c_str();
}

struct dataInfo {
    const BYTE* pdata;
    long length;
};

struct nodeInfo {
    struct dataInfo* pdatainfo;
    const char* ip;
    int  port;
};

struct address {
    short family;
    unsigned short port;
    char ip[100];
    
};

struct dataInfo getFileData(const char* fileName);

bool send_data(int fd, const dataInfo* datainfo);

bool receive_data(int fd, const char *filename);

void output_sockaddr(sockaddr *sa);

void get_addr_from_sockaddr(sockaddr *, address *);

void get_addr_from_fd(int, address *); 

