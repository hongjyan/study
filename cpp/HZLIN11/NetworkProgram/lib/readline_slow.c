#include <unistd.h>
#include <errno.h>
int readline_slow(int fd, void* vptr, size_t maxlen) {
    char c, *ptr = (char*)vptr;
    size_t n = 0;
    ssize_t rc = 0;
    for (n=1; n < maxlen; n++) {
        if ( (rc = read(fd, &c, 1)) == 1) {
            *ptr++ = c;
            if ('\n' == c) break;
        } else if (0 == rc) {
            *ptr = 0; //why?
            return (n-1);
        } 
        else {
            if (errno == EINTR)
                continue;
            return -1;
        }
    }
    *ptr = 0; //why?
    return n;
}
