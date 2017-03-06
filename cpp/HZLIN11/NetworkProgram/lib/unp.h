#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define MAXLINE 4096
#define LISTENQ 1024
#define SERV_PORT 16664
#define SA struct sockaddr
#define max(a,b) ((a) > (b) ? (a) : (b))

typedef void Sigfunc(int);

int Socket(int domain, int type, int protocol);
int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int Listen(int sockfd, int backlog);
int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen); 
ssize_t readline(int fd, void *ptr, size_t maxlen);
int readlinebuf_left(void **ptrptr);
int readn(int fd, void *buf, size_t n);
int Writen(int fd, void* vptr, size_t n);
void sig_chld(int signo);
void sig_pipe(int signo);
//Sigfunc* signal(int signo, Sigfunc* handler);
Sigfunc* Signal(int signo, Sigfunc* handler);
void str_cli(FILE *fp, int sockfd);
void str_echo(int sockfd);
void Printf(const char *str);
int Select(int sockfd, fd_set *rset, fd_set *wset, fd_set *eset, struct timeval *timeout);
