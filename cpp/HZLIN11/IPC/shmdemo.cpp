#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>

#define SHM_SIZE 1024

int main(int argc, char* argv[]) {
    key_t key;
    int shmid;
    char* data;
    char* line;
    size_t n = 0;
    ssize_t read = 0;

    if (argc > 2) {
        printf("argument value number should be less than 3\n");
        exit(-1);
    }

    if (-1 == (key = ftok("sunriver", 's'))) {
        perror("ftok");
        exit(-1);
    }

    if ( -1 == (shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT))) {
        perror("shmget");
        exit(-1);
    }


    data = (char*) shmat(shmid, 0, 0);
    if ((char *) -1 == data) {
        perror("shmat");
        exit(-1);
    }

    //can not trigger share memory write conflict since i can input on 2 windows at the same time.
    if (2 == argc) {
        char* pos = data;
        printf("input something, they will be writen to share memory\n");
        //memory overflow if keep reading too many
        while ( -1 != (read = getline(&line, &n, stdin))) {
            strcpy(pos, line);
            pos += read; 
        }
    }
    else {
        printf("The content of share memory is %s\n", data);
    }

    if (-1 == shmdt(data)) {
        perror("shmdt");
        exit(-1);
    }
    
    return 0;
}
