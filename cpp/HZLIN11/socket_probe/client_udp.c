#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SOCKET_BLOCK 65535
#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 58040

int main(int argc, char **argv)
{
    int sockfd;
    socklen_t src_len;
    socklen_t len;
    struct sockaddr_in dest_addr;
    char send_msg[SOCKET_BLOCK], rece_msg[SOCKET_BLOCK];
    int send_msg_len;
    int iret,i=0;

    //输入参数处理

	if((sockfd=socket(AF_INET,SOCK_DGRAM,0))==-1)
	{
		printf("socket creat failed!\n");
		return 1;
	}
      printf("socket creat succeed!\n");

	memset(&dest_addr, 0, sizeof(dest_addr));
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(SERVER_PORT);
	if(inet_aton(SERVER_ADDR, &dest_addr.sin_addr) < 0)
	{
	printf("[%s] is not a valid IPaddress\n", SERVER_ADDR);
	return 1;
	} 
      printf("inet_aton succeed!\n");

	
	if (connect(sockfd,(struct sockaddr *)&dest_addr,sizeof(dest_addr))<0)
	{
	printf("Connect error\n");
	shutdown(sockfd,2);
	close(sockfd);
	return 1;
	}
	printf("connect succeed!\n");
	
	while(1)
	{
		printf("input data please!\n");

		memset(send_msg,0,sizeof(send_msg));
		send_msg_len = 3;
		memcpy(send_msg,&send_msg_len,2);
		sprintf(send_msg+2,"%c",i);
		printf("send_msg:[%02x %02x %02x]\n",send_msg[0],send_msg[1],send_msg[2]);
		
		if(send(sockfd, send_msg, send_msg_len, 0) < 0)
		{
		perror("sendto error!\n");
		break;;
		return 1;
		}
		printf("send data success!\n");
		
		src_len = sizeof(dest_addr);
		memset(rece_msg,0,sizeof(rece_msg));
		iret = recv(sockfd, rece_msg, sizeof(rece_msg), 0);
		if(iret < 0)
		{
			printf("receive error!\n");
			break;;
			return 1;
		}   
		printf("%s\n",rece_msg);   
	} 
	
	shutdown(sockfd,2);
	close(sockfd);
	
	return 0;
}
