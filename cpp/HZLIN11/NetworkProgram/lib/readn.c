#include "unp.h"

int readn(int fd, void *vptr, size_t n) {
    size_t nleft = n;
    size_t nread = 0;
    char *ptr = (char*)vptr;
    while (nleft > 0) {
        if ( (nread = read(fd, ptr, nleft)) < 0) {
            if (errno == EINTR)
                nread = 0;
            else
                return -1;
        } else if (0 == nread) {
            break;
        }

        nleft -= nread;
        ptr += nread;
    }
    return (n-nleft);
}
   
         
            
