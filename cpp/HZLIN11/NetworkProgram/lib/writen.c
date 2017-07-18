#include <unistd.h>
#include <errno.h>
#include <string.h>

int Writen(int fd, void* vptr, size_t n) {
    const char *ptr = (const char*)vptr;
    size_t nleft = n;
    size_t nwritten = 0;
    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) < 0) {
            if (errno == EINTR)
                nwritten = 0;
            else {
		printf("Writen erro, ERRNO is %s", strerr(errno));
                exit(1);
	    }
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return (n-nleft);
}

       
