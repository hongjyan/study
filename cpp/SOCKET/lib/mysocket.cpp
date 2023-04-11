#include <cstring>
#include "mysocket.h"
#include <ctime>
#include <exception>

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int get_in_port(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return ntohs(((struct sockaddr_in *)sa)->sin_port);
    }
    return ntohs(((struct sockaddr_in6 *)sa)->sin6_port);
}

int get_in_family(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return ((struct sockaddr_in *)sa)->sin_family;
    }
    return ((struct sockaddr_in6 *)sa)->sin6_family;
}

void get_addr_from_sockaddr(sockaddr *sa, address *addr) {
    char s[INET6_ADDRSTRLEN];
    addr->family = get_in_family(sa);
    addr->port = get_in_port(sa);
    inet_ntop(addr->family, get_in_addr(sa), s, sizeof s);
    strcpy(addr->ip, s);
}

void get_addr_from_fd(int fd, address *addr) {
    struct sockaddr sa;
    socklen_t len = sizeof sa;
    if (0 == getsockname(fd, &sa, &len)) {
        return get_addr_from_sockaddr(&sa, addr);
    }
    else {
        printf("getsockname() failed, perhaps socket:%d haven't binded to someaddress.\n", fd);
    }
}

long getFileLength(FILE *file) {
    long lCurPos, lEndPos;
    fseek(file, 0, 0);
    lCurPos = ftell(file);
    fseek(file, 0, 2);
    lEndPos = ftell(file);
    rewind(file);
    return lEndPos-lCurPos;
} 


struct dataInfo getFileData(const char* fileName) {
//    printf("reading data from file into BYTE buffer\n");
    FILE *pFile = NULL;
    if ((pFile = fopen(fileName, "r")) == NULL) {
        printf("!!!!open file failed.\n");
        exit(-1);
    }
    const long fileLength = getFileLength(pFile);
//    printf("fileLength is %ld\n", fileLength);
    int longLen = sizeof fileLength;
    const long bytes_data = fileLength + longLen;
    BYTE *pdata = new BYTE [bytes_data];
    //    vector<unsigned char> length_byte = longToByte(fileLength);
    std::copy(static_cast<const char*>(static_cast<const void*>(&fileLength)),
              static_cast<const char*>(static_cast<const void*>(&fileLength)) + longLen,
              pdata);
    fread(pdata+longLen, fileLength, 1, pFile);
    fclose(pFile);
    struct dataInfo datainfo;
    datainfo.pdata = pdata;
    datainfo.length = bytes_data;
    return datainfo;
}

bool recv_n(int fd, int n, char* buffer) { //user guarantee buffer's size. need to add method to get security.
    if (NULL == buffer) return false;
    ssize_t received = 0,
    total_received = 0,
    rest = n;
        
 
    do {
        received = recv(fd, buffer+received, rest, 0);
        if (-1 == received) {
            if (11 == errno) { 
                static int i = 0;
                printf("errno is EAGIAIN:11 for socket:%d, so try agin\n", fd);
                received = 0;
                i++;
                if (30 == i) {
                    printf("socket:%d recv timeout already 30 times, won't try again\n", fd);
                    return false;
                }
                continue;
            }
            printf("<------------------------------------------------------------------------------------------------------\n");
            printf("receive failed, errno is %d, errmsg is %s\n", errno, strerror(errno));
            return false;
        }
        else if (0 == received) {
            printf("<------------------------------------------------------------------------------------------------------\n");
            printf("receive timeout, errno is %d, errmsg is %s\n", errno, strerror(errno));
            return false;
        }
        else {
            total_received += received;
            rest = n - total_received;
        }
    }
    while (0 != rest);
    return true;
}




bool send_data(int fd, const dataInfo* datainfo) {
    int bytes_sent = 0,
        total_bytes_sent = 0,
        rest = datainfo->length;

    struct address addr; 
    get_addr_from_fd(fd, &addr);

    do {    
        bytes_sent = send(fd, datainfo->pdata + total_bytes_sent, rest, 0);
        if (bytes_sent == -1) { 
            printf("<------------------------------------------------------------------------------------------------------\n");
            printf("!!!!socket:%d for %d-%s-%d sending data failed. Errno is %d:%s\n", fd, addr.family, addr.ip, addr.port, errno, strerror(errno));
            return false;
         }
         else if (bytes_sent == 0) {
            printf("<------------------------------------------------------------------------------------------------------\n");
            printf("!!!!socket:%d for %d-%s-%d sending data timeout. Errno is %d:%s\n", fd, addr.family, addr.ip, addr.port, errno, strerror(errno));
            return false;
         }
         total_bytes_sent += bytes_sent;
         rest -= bytes_sent;
    }
    while (rest != 0);
    return true;       
}


bool receive_data(int fd, const char *filename) {        
    long recvdata_len = 0;
    ssize_t long_len = sizeof recvdata_len;
    struct sockaddr srvAddr;
    socklen_t len = sizeof srvAddr;
    struct address myAddrInfo, srvAddrInfo;

    get_addr_from_fd(fd, &myAddrInfo);
    getpeername(fd, &srvAddr, &len); 
    get_addr_from_sockaddr(&srvAddr, &srvAddrInfo);
    
/*
    struct timeval tv;
    tv.tv_sec = 40;
    tv.tv_usec = 0;
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);
    switch (select(fd+1, &rfds, NULL, NULL, &tv)) {
        case -1:
            printf("Error happened during select for socket:%d for client:%hd-%s:%hu--------server:%hd-%s:%hu , errno is %d, errmsg is %s", 
                   fd, myAddrInfo.family, myAddrInfo.ip, myAddrInfo.port, srvAddrInfo.family, srvAddrInfo.ip, srvAddrInfo.port, errno, strerror(errno));
            return false;
        case 0:
            continue;
        default:
            if (!FD_ISSET(fd, &rfds)) return false;
            break;
    }
*/
    if (!recv_n(fd, long_len, (char*) &recvdata_len)) {
        printf("!!!!get file length for socket:%d for client:%hd-%s:%hu--------server:%hd-%s:%hu failed\n", 
	       fd, myAddrInfo.family, myAddrInfo.ip, myAddrInfo.port, srvAddrInfo.family, srvAddrInfo.ip, srvAddrInfo.port);
        return false;
    }


    char incoming_data_buffer[recvdata_len];
    if (!recv_n(fd, recvdata_len, incoming_data_buffer)) {
        printf("!!!!recv data for socket:%d for client:%hd-%s:%hu--------server:%hd-%s:%hu failed\n", 
	       fd, myAddrInfo.family, myAddrInfo.ip, myAddrInfo.port, srvAddrInfo.family, srvAddrInfo.ip, srvAddrInfo.port);
        return false;
    }


    try {
        FILE *pfile = fopen(filename, "a+");
        std::time_t result = std::time(NULL);
        const char* now = std::asctime(std::localtime(&result));
        fwrite(now, 25, 1, pfile);
//        fwrite(incoming_data_buffer, total_bytes_received, 1, pfile);
        fclose(pfile);
    }
    catch (std::exception& e) {
        printf("%s\n", e.what());
        return false;
    }

    return true;
}



bool isblocking(int fd) {
    long args;
    args = fcntl(fd, F_GETFL, NULL);
    return !(args&O_NONBLOCK);
}
