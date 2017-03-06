#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc, char* argv[]) {
    int fd,
        offset;
    char* data;
    struct stat sbuf;

    if (argc != 2) {
        printf("Usage: mmapdemo offset\n");
        exit(-1);
    }

    if ( -1 == (fd = open("sunriver", O_RDWR))) {
        perror("open");
        exit(-1);
    }

    if (-1 == stat("sunriver", &sbuf)) {
        perror("stat");
        exit(-1);
    }

    offset = atoi(argv[1]);
    if ((char*) -1 == (data = (char*) mmap(0, sbuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0))) {
        perror("mmap");
        exit(-1);
    }


    printf("1st: data[%d] is %c\n", offset, data[offset]);
    printf("input one character\n");
    data[offset] = getchar();
    printf("2nd: data[%d] is %c\n", offset, data[offset]);


    close(fd);

    printf("getchar() to get a char to exit\n");
    getchar();
    getchar();
    return 0;
}
    

