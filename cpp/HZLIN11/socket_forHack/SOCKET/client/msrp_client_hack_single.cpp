#include <cstring>  //memset()
#include <algorithm> //std::copy()
#include <pthread.h> //pthread_create()
#include <poll.h>
#include <ctime>  //std::time
#include "../lib/mysocket.h"



void* client(void* pNode)
{
    struct nodeInfo *pnode = static_cast<struct nodeInfo *>(pNode);
    int status;
    struct addrinfo host_info;
    struct addrinfo *host_info_list, *p;
    char port[30];
    sprintf(port, "%d", pnode->port);
    char filename[100];
    sprintf(filename, "%s_%s", pnode->ip, port);
    int rcvbuf = 65535;
    int sndbuf = 65535;

    memset(&host_info, 0, sizeof host_info);

    host_info.ai_family = AF_UNSPEC; 
    host_info.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(pnode->ip, port, &host_info, &host_info_list);

    if (status != 0) {
         printf("!!!!getting info for %s : %s failed\n", pnode->ip, port);
         delete pnode;
         pthread_exit(NULL);
    }



    int socketfd;
    for (p=host_info_list; p!=NULL; p=p->ai_next) {
	if ((socketfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) continue;
	break;
    }

    if (p==NULL) {
	printf("!!!!creating socket for %s : %s failed\n", pnode->ip, port);
        delete pnode;
	pthread_exit(NULL);
    }

    struct timeval tv;
    tv.tv_sec = 40;
    tv.tv_usec = 0;
    if (setsockopt(socketfd, SOL_SOCKET, SO_RCVTIMEO, (char*) &tv, sizeof tv)) {
        close(socketfd);
        delete pnode;
        pthread_exit(NULL);
    }

    if (setsockopt(socketfd, SOL_SOCKET, SO_RCVBUF, &rcvbuf, sizeof(int))) {
        close(socketfd);
        delete pnode;
        pthread_exit(NULL);
    }

    if (setsockopt(socketfd, SOL_SOCKET, SO_SNDBUF, &sndbuf, sizeof(int))) {
        close(socketfd);
        delete pnode;
        pthread_exit(NULL);
    }

/*
    struct sockaddr_in bindAddr;
    memset(&bindAddr, 0, sizeof bindAddr);
    bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bindAddr.sin_family = AF_INET;
    bindAddr.sin_port = htons(0);
    if (bind(socketfd, (sockaddr *)&bindAddr, sizeof bindAddr)) {
       delete pnode;
       printf("!!!!bind client socket:%d for server:%s:%s failed, errmsg is %s\n", 
                    socketfd, pnode->ip, port, strerror(errno));
       pthread_exit(NULL);
    }
*/

    struct address clientaddr;
    

    if (0 != connect(socketfd, p->ai_addr, p->ai_addrlen)) { 
        printf("!!!!socket:%d connet server:%s:%s peer failed, errmsg is %s\n", 
                    socketfd, pnode->ip, port, strerror(errno));
        close(socketfd);
        delete pnode;
        pthread_exit(NULL);
    }
    std::time_t stime = std::time(NULL),
                etime = 0;
/*
    int connTimes = 9;    //try connect in 45s
    int rc = -1;
    while (connTimes--) {
        if ((rc = connect(socketfd, p->ai_addr, p->ai_addrlen)) == 0) break;
        usleep(5000000);
    }
    if (connTimes == -1) {
            printf("!!!!socket:%d connet server:%s:%s peer failed, errmsg is %s\n", 
                    socketfd, pnode->ip, port, strerror(errno)); 
            close(socketfd);
            delete pnode;
	    pthread_exit(NULL);
    }
*/    
           
    freeaddrinfo(host_info_list);

    get_addr_from_fd(socketfd, &clientaddr); 
   
    
    int count = 500; // 500 times * 20 ms = 10s
    //while(true) {
    //    printf("Entering while\n");
//    receive_data(socketfd, filename);
    //	usleep(20000000);  //interval is 20ms.
    //}
    printf("out of while\n");

/*    
    if (-1 != count) {
        printf("Error happen during receive/send at %d times for socket:%d server:%s:%s------->client:%s:%d\n", 
        count+1, socketfd, pnode->ip, port, clientaddr.ip, clientaddr.port);
        printf("------------------------------------------------------------------------------------------------------>\n");
    }


    etime = std::time(NULL);
    unsigned int speed = (409 - count) * 160 / (etime - stime);
    FILE *pfile = fopen("speed_client", "a+");
    char temp[200];
    if (-1 == count) {
        sprintf(temp, "socket speed for %s:%d is %uByte/s\n", pnode->ip, pnode->port, speed);
    }
    else {
        sprintf(temp, "!!!socket speed for %s:%d is %uByte/s\n", pnode->ip, pnode->port, speed);
    }    
    fwrite(temp, strlen(temp), 1, pfile);
    fclose(pfile);
*/

    close(socketfd);
    delete pnode;
    return p->ai_addr;
}

void usage(void) {
    printf("usage:\n    msrp_client.o $ip $startPort, $numPort, $file_to_sent\n");
}

int main(int argc, char *argv[]) 
{
    if (argc < 5) {
        usage();
        exit(-1);
    }

    const char* ip = argv[1];
    const int startPort = atoi(argv[2]);
    const int numPort = atoi(argv[3]);
    const char* file_to_send = argv[4];
    int i, rc;
    pthread_t threads[numPort];
    
//    struct dataInfo datainfo = getFileData(file_to_send);
    struct dataInfo datainfo = getData();

    printf("numPort is %d\n", numPort);
    for (i=0; i<numPort; i++) {
        struct nodeInfo *pnodeInfo = new nodeInfo();
        pnodeInfo->pdatainfo = &datainfo;
        pnodeInfo->ip = ip;
        pnodeInfo->port = startPort;
        if (0 != (rc = pthread_create(&threads[i], NULL, client, pnodeInfo))) {
            printf("!!!!thread for %s:%d creation failed at %d times\n", ip, startPort, i);
            if (pnodeInfo) delete pnodeInfo;
	}
    }

    for (i=0; i<numPort; i++) {
       pthread_join(threads[i], NULL);
    }
    delete [] datainfo.pdata;
}
