/* include fig01 */
#include	"unp.h"

int
main(int argc, char **argv)
{
	int					i, maxi, maxfd, listenfd, connfd, sockfd;
	int					nready, client[FD_SETSIZE];
	ssize_t				n;
	fd_set				rset, allset;
	char				buf[MAXLINE];
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr;
	int yes = 1;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);


	maxfd = listenfd;			/* initialize */
	maxi = -1;					/* index into client[] array */
	for (i = 0; i < FD_SETSIZE; i++)
		client[i] = -1;			/* -1 indicates available entry */
	FD_ZERO(&allset);
	FD_ZERO(&rset);
	FD_SET(listenfd, &allset);
/* end fig01 */

/* include fig02 */
	int j = 0;
	for ( ; ; ) {
		printf("0.1\n");
		rset = allset;		/* structure assignment */
		printf("0.2\n");
		//It is different for the 2 lines below. If take allset(right value of line "rset = allset"), then right-value will be modify, but the assignment take the right-value as their common result. it is not our expected here. Here, we use left-value because it doesn't matter left-value be modified or not, since left-value will be modified at assignment.
		nready = Select(maxfd+1, &rset, NULL, NULL, NULL); 
		//nready = Select(maxfd+1, &allset, NULL, NULL, NULL);

		if (FD_ISSET(listenfd, &rset)) {	/* new client connection */
			clilen = sizeof(cliaddr);
			connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);
			for (i = 0; i < FD_SETSIZE; i++) {
				if (client[i] < 0) {
					client[i] = connfd;	/* save descriptor */
					break;
				}
                        }
			printf("2\n");
			if (i == FD_SETSIZE)
				err_quit("too many clients");

			printf("3\n");
			FD_SET(connfd, &allset);	/* add new descriptor to set */
			printf("4\n");
			if (connfd > maxfd)
				maxfd = connfd;			/* for select */
			if (i > maxi)
				maxi = i;				/* max index in client[] array */

			if (--nready <= 0) {
				printf("j is %d\n", j);
				j++;
				continue;				/* no more readable descriptors */
			}
			printf("5\n");
			printf("FD_ISSET(connfd, &allset) is %d\n", FD_ISSET(connfd, &allset));
		}

		for (i = 0; i <= maxi; i++) {	/* check all clients for data */
			printf("6\n");
			if ( (sockfd = client[i]) < 0)
				continue;
			if (FD_ISSET(sockfd, &rset)) {
				if ( (n = Read(sockfd, buf, MAXLINE)) == 0) {
						/*4connection closed by client */
                                        printf("here\n");
					Close(sockfd);
					FD_CLR(sockfd, &allset);
					client[i] = -1;
				} else
					Writen(sockfd, buf, n);
				
				//jump when 1. the first connected but not ready socket && 2.iterator all ready sockets
				if (--nready <= 0)
					break;				/* no more readable descriptors */
			}
		}
	}
}
/* end fig02 */
