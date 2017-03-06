extern "C"
{
    #include "unp.h"
}


int main(int argc, char* argv[]) {
    int i, sockfd[5];
    struct sockaddr_in servaddr;

    if (2 != argc) {
	printf("usage: tcpcli IPaddress\n");
	return -1;
    }


    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    
    for (i = 0; i < 2; i++) {
	sockfd[i] = Socket(AF_INET, SOCK_STREAM, 0);
	//Connect(sockfd[i], (SA *) &servaddr, sizeof servaddr);
	try {
	    if (connect(sockfd[i], (SA *) &servaddr, sizeof servaddr) < 0) 
		Printf("connect failed");
	} catch (...) {
	    printf("Caugth a exception\n");
	}
    }

    printf("1\n");
    str_cli(stdin, sockfd[0]);

    return 0;
}

