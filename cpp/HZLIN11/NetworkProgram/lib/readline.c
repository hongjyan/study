#include <unistd.h>
#include <errno.h>
#define MAXLINE 5000

static int read_cnt = 0;
static char readbuf[MAXLINE];
static char *read_ptr;

static ssize_t my_read(int fd, char *ptr) { 
    if (read_cnt <= 0 ) {
again:
        read_cnt = read(fd, readbuf, sizeof(readbuf));
        if (0 > read_cnt) {
            if (EINTR == errno)
		goto again;
            return -1;
        } else if (0 == read_cnt) {
            return 0;
        }
        read_ptr = readbuf;
    }

    *ptr = *read_ptr++;
    read_cnt--;
    return 1;
}


ssize_t readline(int fd, void *vptr, size_t maxlen) {
    size_t n, rc;
    char   c, *ptr;

    ptr = vptr;
    for (n=1; n<=maxlen; n++) {
        if (1 == (rc = my_read(fd, &c))) {
            *ptr++ = c;
            if ('\n' == c) 
		break;
        }
        else if (0 == rc) {
	    *ptr = 0;
	    return (n-1);
	}
        else 
	    return -1;
    }

    *ptr = 0;
    return n;
}    

int readlinebuf_left(void **read_ptr_ptr) {
    if (read_cnt)
	*read_ptr_ptr = read_ptr;
    return read_cnt;
}
