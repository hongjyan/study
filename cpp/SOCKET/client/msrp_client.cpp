#include <cstring>  //memset()
#include <algorithm> //std::copy()
#include <pthread.h> //pthread_create()
#include <poll.h>
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
    int rcvbuf = 262142;

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
        delete pnode;
        pthread_exit(NULL);
    }

    if (setsockopt(socketfd, SOL_SOCKET, SO_RCVBUF, &rcvbuf, sizeof(int))) {
        delete pnode;
        pthread_exit(NULL);
    }

    struct address clientaddr;
    

    if (0 != connect(socketfd, p->ai_addr, p->ai_addrlen)) { 
        printf("!!!!socket:%d connet server:%s:%s peer failed, errmsg is %s\n", 
                    socketfd, pnode->ip, port, strerror(errno));
        close(socketfd);
        delete pnode;
        pthread_exit(NULL);
    }
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
    while (count--) {
        if (!receive_data(socketfd, filename))  break;
        if (!send_data(socketfd, pnode->pdatainfo)) break;
        usleep(20000);  //interval is 20ms.
    }
    
    if (-1 != count) {
        printf("Error happen during receive/send at %d times for socket:%d server:%s:%s------->client:%s:%d\n", 
        count+1, socketfd, pnode->ip, port, clientaddr.ip, clientaddr.port);
        printf("------------------------------------------------------------------------------------------------------>\n");
    }

    
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
    
    struct dataInfo datainfo = getFileData(file_to_send);

    for (i=0; i<numPort; i++) {
        struct nodeInfo *pnodeInfo = new nodeInfo();
        pnodeInfo->pdatainfo = &datainfo;
        pnodeInfo->ip = ip;
        pnodeInfo->port = startPort+i;
        if ((rc = pthread_create(&threads[i], NULL, client, pnodeInfo))) {
            printf("!!!!thread for %s:%d creation failed\n", ip, startPort+i);
            if (pnodeInfo) delete pnodeInfo;
	}
    }

    for (i=0; i<numPort; i++) {
       pthread_join(threads[i], NULL);
    }
    delete [] datainfo.pdata;
}
