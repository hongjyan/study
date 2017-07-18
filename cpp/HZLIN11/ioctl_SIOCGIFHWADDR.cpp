#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char* argv[]) {
    const char* ipaddr = argv[1];
    int sd;
    struct ifreq ifr;
    sockaddr_in6 sockaddr;
    unsigned char *p;

    if (0 > (sd = socket(AF_INET6, SOCK_RAW, IPPROTO_ICMPV6))) {
        perror("socket()");
        exit(-1);
    }

    if ( 0 > inet_pton(AF_INET6, ipaddr, &(sockaddr.sin6_addr))) {
        perror("inet_pton");
        exit(-1);
    }

    memset(&ifr, 0, sizeof(ifr));
    memcpy(&(ifr.ifr_addr), &(sockaddr.sin6_addr), sizeof(ifr.ifr_addr));
    
    if (0 > ioctl(sd, SIOCGIFHWADDR, &ifr)) {
        perror("ioctl");
        exit(-1);
    }

    p = (unsigned char*) &(ifr.ifr_hwaddr);
    for (int i=0; i<5; i++) {
        printf("%0x ", *p);
        p++;
    }
    printf("%0x\n", *p);
   
    return 0;
}
