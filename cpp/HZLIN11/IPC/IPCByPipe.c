#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int fds[2];
    char writeBuf[30];
    char readBuf[30];
    pipe(fds);

    if (!fork()) {
        printf("Enter something into child\n");
        scanf("%s", writeBuf);
        write(fds[1], writeBuf, 30);
    }
    else {
        read(fds[0], readBuf, 30);
        printf("what was read from child is:\n%s\n", readBuf);
    }
    wait(NULL);
} 
        
