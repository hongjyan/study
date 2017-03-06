#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include "fileLockIM.h"

#define DEBUG 1

int main() {
    int rfd = open(FILERD, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
    int wfd = open(FILEWR, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
    if (-1 == rfd) { perror("open"); exit(-1); }
    if (-1 == wfd) { perror("open"); exit(-1); }

    char msgsend[2000]; 
    char msgrcv[2000];

    pid_t pid = fork();
    if (0 == pid) {
        int readcnt = 0;
        while (true) {
            readcnt = readFromFile(rfd, msgrcv, 2000);
            if (readcnt) printf("<:%s\n", msgrcv);
        }         
        #ifdef DEBUG 
            printf("Jump out of while-read, about to close fd\n");
        #endif   
        close(rfd);
    }
    else if (0 < pid) {
        while (true && !feof(stdin)) {
            printf(">:\n");
            if (NULL == fgets(msgsend, 1024, stdin)) { perror("fgets"); close(wfd); exit(-1); }
            if (!writeToFile(wfd, msgsend, strlen(msgsend)+1)) { perror("writeToFile"); close(wfd); exit(-1);}
        }
        close(wfd);
        //wait(NULL) // not wait here to terminate child process when use enter "Ctrl+D"
        return 0;
    }
    else {
        perror("fork");
        close(rfd);
        close(wfd);
        open(FILERD, O_WRONLY|O_TRUNC, 0);
        open(FILEWR, O_WRONLY|O_TRUNC, 0);
        close(rfd);
        close(wfd);
        return -1;
    }
}
