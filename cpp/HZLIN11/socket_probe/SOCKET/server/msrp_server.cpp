#include <cstring> //memset, strlen
#include <unistd.h> //close(socketfd)
#include <sys/types.h>  //sigaction(), WNOHANG
#include <algorithm> //std::copy
#include <pthread.h>
#include <ctime>
#include "../lib/mysocket.h" 


void* server(void* pNode) {
    struct nodeInfo *pnode = static_cast<struct nodeInfo *>(pNode);
    int sockfd, new_fd, rv;
    struct addrinfo hints, *serverinfo_list, *p;
    struct sockaddr clientaddr;
    int yes=1;
    int rcvbuf = 65535;
    int sndbuf = 65535;

    char port[30];
    sprintf(port, "%d", pnode->port);
    char filename[100];
    sprintf(filename, "%s_%s", pnode->ip, port);

    memset(&hints, 0, sizeof(addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM | (!O_NONBLOCK);
    struct timeval tv;
    tv.tv_sec = 40;
    tv.tv_usec = 0;

    if ((rv = getaddrinfo(pnode->ip, port, &hints, &serverinfo_list)) != 0) {
	printf("!!!!get_addr_info failed for %s:%s, reason is %s\n", pnode->ip, port, gai_strerror(rv));
        delete pnode;
        pthread_exit(NULL);
    };
    
    
    for (p=serverinfo_list; p!=NULL;  p=p->ai_next) {
	if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            printf("!!!!create socket failed\n");    
            continue;
        }
	
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
	    printf("!!!!set socket option failed for socket:%d for %s:%s\n", sockfd, pnode->ip, port);
            close(sockfd);
            delete pnode;
            pthread_exit(NULL);
	}

	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcvbuf, sizeof(int)) == -1) {
	    printf("!!!!set socket option failed for socket:%d for %s:%s\n", sockfd, pnode->ip, port);
            close(sockfd);
            delete pnode;
            pthread_exit(NULL);
	}

	if (setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &sndbuf, sizeof(int)) == -1) {
	    printf("!!!!set socket option failed for socket:%d for %s:%s\n", sockfd, pnode->ip, port);
            close(sockfd);
            delete pnode;
            pthread_exit(NULL);
	}


       if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof tv) == -1) {
           close(sockfd);
           printf("!!!!set socket option failed for socket:%d for %s:%s\n", sockfd, pnode->ip, port);
           delete pnode;
           pthread_exit(NULL);
       }

	if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
	    printf("!!!!bind socket failed in loop for socket:%d for %s:%s. Errno is %d, %s\n", sockfd, pnode->ip, port, errno, strerror(errno));
	    close(sockfd);
	    continue;
	}
	break;
    }

    if (p==NULL) {
	printf("!!!!create/setsockopt/bind socket for %s:%s failed\n", pnode->ip, port);
        delete pnode;
	pthread_exit(NULL);
    }

    freeaddrinfo(serverinfo_list);

    if ((listen(sockfd, 1)) == -1) {
	printf("!!!!listening failed on socket:%d for %s:%s. Errno is %d, %s\n", sockfd, pnode->ip, port, errno, strerror(errno));
        delete pnode;
        close(sockfd);
	pthread_exit(NULL);
    }



    int nready;
    struct timeval timeout;
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(sockfd, &rfds);
    timeout.tv_sec = 80;
    timeout.tv_usec = 0;
    nready = select(sockfd+1, &rfds, NULL, NULL, &timeout);
    switch (nready) {
	case -1 :
            close(sockfd);
	    printf("!!!!Error happened during select on socket:%d for %s:%s\n", sockfd, pnode->ip, port);
            delete pnode;
	    pthread_exit(NULL);
	case 0 :
            close(sockfd);
	    printf("Time experied during select on socket:%d for %s:%s\n", sockfd, pnode->ip, port);
            delete pnode;
	    pthread_exit(NULL);
	default:
            if (!FD_ISSET(sockfd, &rfds)) {
                close(sockfd);
	        printf("select() return postive value, but socket:%d for %s:%s is not in rfds \n", sockfd, pnode->ip, port);
                delete pnode;
                pthread_exit(NULL);
            }
            break;
    }


    socklen_t sin_size = sizeof clientaddr;
    if ((new_fd = accept(sockfd, &clientaddr, &sin_size)) == -1) {
        printf("!!!!accept on socket:%d for %s:%s failed, errno is %s\n", sockfd, pnode->ip, port, strerror(errno));
        close(sockfd);
    	delete pnode;
        pthread_exit(NULL);
    }
    else {
        close(sockfd);
    }


    int count = 500;
    std::time_t stime = std::time(NULL),
                etime = 0;

    while (0 != count--) {
    	if (!send_data(new_fd, pnode->pdatainfo)) break;
        if (!receive_data(new_fd, filename)) break; 
        usleep(20000);
    }
    if (-1 != count) {
        printf("send/receive data failed at %d times for %s:%d\n", count+1, pnode->ip, pnode->port);
        printf("------------------------------------------------------------------------------------------------------>\n");
    }

    etime = std::time(NULL);
    unsigned int speed = (409 - count) * 160 / (etime - stime);
    FILE *pfile = fopen("speed_server", "a+");
    char temp[200];
    if (-1 == count) {
        sprintf(temp, "socket speed for %s:%d is %uByte/s\n", pnode->ip, pnode->port, speed);
    }
    else {
        sprintf(temp, "!!! socket speed for %s:%d is %uByte/s\n", pnode->ip, pnode->port, speed);
    }    
    fwrite(temp, strlen(temp), 1, pfile);
    fclose(pfile);
  
    close(new_fd);
    delete pnode;
    return p->ai_addr;
}


void usage(void) {
    printf("usage:\n    msrp_server.o $ip $startPort, $numPort, $file_to_sent\n");
}


int main(int argc, char *argv[]) {
    if (argc < 5) {
        int i = 0;
        while (i<argc) {
          printf("%s ", argv[i]);
          i++;
        }
        printf("\n");
        
        usage();
        exit(-1);
    }

    const char* ip = argv[1];
    const int startPort = atoi(argv[2]);
    const int numPort = atoi(argv[3]);
    const char* file_to_send = argv[4];
    pthread_t threads[numPort];
    int i, rc;
    
//    struct dataInfo datainfo = getFileData(file_to_send);
    struct dataInfo datainfo = getData();

    for (i=0; i<numPort; i++) {
        struct nodeInfo *pnodeInfo = new nodeInfo();
        pnodeInfo->pdatainfo = &datainfo;
        pnodeInfo->ip = ip;
        pnodeInfo->port = startPort+i;
        if ((rc = pthread_create(&threads[i], NULL, server, pnodeInfo))) {
            printf("!!!!thread created failed for %s:%d", ip, startPort+i);
            if (pnodeInfo) delete pnodeInfo;
        }
    }

    for (i=0; i<numPort; i++) {
        pthread_join(threads[i], NULL);
    }
    delete [] datainfo.pdata;
}

