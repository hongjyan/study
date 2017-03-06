#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>

#define SHM_SIZE 1024

int main() {
    key_t key;
    int shmid;

    key = ftok("sunriver", 's');
    
    if ( -1 == (shmid = shmget(key, SHM_SIZE, 0))) {
        perror("shmget failed");
        exit(-1);
    }

    if (-1 == shmctl(shmid, IPC_RMID, NULL)) {
        perror("shmctl remove failed");
        exit(-1);
    }

    return 0;
}
