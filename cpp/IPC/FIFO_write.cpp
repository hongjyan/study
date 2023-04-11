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
   
    // In this case, the dev parameter is being set to zero because it is not relevant for named pipes.
    // The dev parameter is only meaningful when creating special files, such as character or block devices, but not for named pipes.
    if ( -1 == (mknod(FIFO_NAME, S_IFIFO | 0666, 0))) perror("mknod failed");
   
    printf("waiting for reader\n");
    // open()函数以只写模式打开了一个名为"china_maid"的命名管道（FIFO）。如果此时没有读进程打开该管道，则open()函数将阻塞，等待读进程打开管道。
    // 这是因为在读进程打开管道之前，操作系统会将该管道设为阻塞模式。此时，如果没有进程尝试从管道中读取数据，写进程将被阻塞，直到有读进程打开管道并开始读取数据。这是为了确保写进程不会向没有读进程的管道中写入数据，从而造成数据丢失。
    // 因此，在这个程序中，如果没有读进程打开管道，open()函数将一直阻塞，直到有读进程打开管道。
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
