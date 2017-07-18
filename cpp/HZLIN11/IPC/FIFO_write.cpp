#include <stdio.h>    //gets, printf
#include <sys/types.h>  //mknod
#include <sys/stat.h>   //mknod
#include <stdio.h>    //gets, printf
#include <sys/types.h>  //mknod
#include <sys/stat.h>   //mknod
#include <fcntl.h>      //mknod, open 
#include <unistd.h>     //write
#include <string.h>     //strlen


#define FIFO_NAME "china_maid"



int main() {
    char s[300];
    int fd, num;
   
    if ( -1 == (mknod(FIFO_NAME, S_IFIFO | 0666, 0))) perror("mknod failed");
   
    printf("waiting for reader\n");
    fd = open(FIFO_NAME, O_WRONLY);

    printf("got a reader----type something\n");

    while (fgets(s, 299, stdin) && (!feof(stdin))) {  //Ctrl+D == feof(stdin)
        if ((num = write(fd, s, strlen(s))) == -1) {
            perror("write");
        }
        else {
            printf("write %d bytes to FIFO\n", num);
        }
    }

    return 0;  //curiosly, can not use cat to output content of "china_maid"
}
