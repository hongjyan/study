#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <netinet/ip6.h>

#define MAX_BUFFER 65535

void usage() {
    printf("./raw_client.o <src_addr> <dst_addr>\n");
    exit(-1);
}


int main(int argc, char* argv[]) {
    if (argc != 3) usage();
    
    char buffer[MAX_BUFFER];
    int sockfd;
    int on = 1;
    int sentnum = 0;
    int recvBufferSize = 65535;
    unsigned short ipv6payloadlen = 48;
    struct sockaddr  *p_srcaddr;
    struct sockaddr_in6 dstaddr;
    socklen_t socklen = sizeof(struct sockaddr);
    struct addrinfo hints, *p_node_list, *p;
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_RAW;
    //hints.ai_protocol = IPPROTO_ICMPV6;


    if (0 != getaddrinfo(argv[1], 0, &hints, &p_node_list)) {
        perror("getaddrinfo");
        exit(-1);
    }
    
    if (1 != inet_pton(AF_INET6, argv[2], (sockaddr*)&dstaddr)) {
        perror("inet_pton");
        exit(-1);
    } 

    for (p=p_node_list; p!= NULL; p = p->ai_next) {
        if (-1 != (sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol))) {
            p_srcaddr = p->ai_addr;
            break;
        }
    }

    if (NULL == p) {
        printf("create socket failed\n");
        exit(-1);
    }

    freeaddrinfo(p_node_list);

    if (-1 == setsockopt(sockfd, IPPROTO_IPV6, 36, &on, sizeof(on))) {
        perror("setsockopt for IPV6_HDRINCL");
        exit(-1);
    }


    if (-1 == setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &recvBufferSize, sizeof(recvBufferSize))) {
        perror("setsockopt for SO_RCVBUF");
        exit(-1);
    }
    
   
    if (-1 == setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) {
        perror("setsockopt for SOL_SOCKET");
        exit(-1);
    }

    if (-1 == bind(sockfd, p_srcaddr, socklen)) {  //ok here? socklen = sizeof(struct sockaddr) which is 16, but sockaddr_in6 is larger than 16.
        perror("bind");
        exit(-1);
    }
 



    memset(buffer, 0, MAX_BUFFER);
    struct ip6_hdr *ip6 = (struct ip6_hdr*)buffer;
    ip6->ip6_plen = htons(ipv6payloadlen);
    ip6->ip6_nxt = 0xff; //wrong next header intendedly
    ip6->ip6_hops = 0x64;
    ip6->ip6_vfc = (6<<4);
    ip6->ip6_src = ((struct sockaddr_in6*)p_srcaddr)->sin6_addr;
    ip6->ip6_dst = dstaddr.sin6_addr;
    memset(buffer+sizeof(struct ip6_hdr), 0x5a, ipv6payloadlen);
    
    socklen = sizeof(dstaddr);
    sentnum = sendto(sockfd, buffer, sizeof(struct ip6_hdr)+ipv6payloadlen, 0, (sockaddr*)&dstaddr, socklen);

    if (-1 == sentnum) {
        perror("sentto");
    } else {
        printf("sent %d bytes\n", sentnum);
    }
   
    return 0;    
}
