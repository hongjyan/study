#include <stdio.h>
#include <sys/sem.h>
#include <stdlib.h>

union semun {
    int val;
    struct semid_ds *buf;
    int *array;
};


int main() {
    key_t key;
    int semid;
    union semun arg;


    key = ftok("sunriver", 'S');
    if ( -1 == (semid = semget(key, 20, 0))) { 
        perror("semget failed");
        exit(-1);
    }


    if ( -1 == semctl(semid, 0, IPC_RMID, arg)) {
        perror("remove semaphore failed");
        exit(-1);
    }
    
    return 0;
}
    
    
