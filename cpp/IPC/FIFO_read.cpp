#include <stdio.h>
#include <sys/types.h>  //mknod
#include <sys/stat.h>   //mknod
#include <fcntl.h>      //mknod, open 
#include <unistd.h>     //write
#include <string.h>     //strlen


#define FIFO_NAME "china_maid"

int main() {
    char s[300];
    int fd, num;
   
    if (mknod(FIFO_NAME, S_IFIFO| 0666, 0) == -1) perror("mknod");

    printf("Waiting for a writer\n");
    fd = open(FIFO_NAME, O_RDONLY);
    printf("Got a writer\n");
    
    do {
        if ((num = read(fd, s, 300)) == -1) {
            perror("read");
        }
        else {
          s[num] = '\0';
          printf("%s\n", s);
          printf("read %d bytes\n", num);
        }
    } while (num > 0);
    
    return 0;
}
