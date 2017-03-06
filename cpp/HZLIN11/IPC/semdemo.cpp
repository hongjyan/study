#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <errno.h>

#define MAX_RETRIES 10


union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
}; // do not know why semun need customore to define. But from man semctl, it says: The  calling program must define this union as follows:

int initsem(key_t key, int nseems) {
    union semun semunion;
    struct semid_ds state;
    struct sembuf buf;
    int semid;
    bool ready = false;


    buf.sem_num = 0;
    buf.sem_op = 1;
    buf.sem_flg = 0;

    semid = semget(key, nseems, IPC_CREAT | IPC_EXCL | 0666);

    if (semid > 0) {
        for (; buf.sem_num < nseems; buf.sem_num++) {
            if ((semop(semid, &buf, 1)) == -1) {
                perror("semop");
                semctl(semid, 0, IPC_RMID);
                return -1;
            }
        }
        return semid;
    }
    else if (errno == EEXIST) {
        semid = semget(key, nseems, 0);
        if (semid == -1) { perror("get semaphores ID from other process failed "); return -1; }
        
        for (int i=0; i < MAX_RETRIES; i++) {
            semunion.buf = &state; //make semun 's member be "semid_ds" type
            semctl(semid, nseems-1, IPC_STAT, semunion); //the last arg of semctl when IPC_STAT specified should be semun; but semun's member:buf which has type semid_ds will be assigned during semctl call. 
            if (state.sem_otime != 0) {  //state is assigned after call above.
                ready = true; 
                break;
            }
            else {
                sleep(1);
            }
       }

       if (!ready) {
           perror("waiting for other process initial semaphores timeout");
           return -1;
       }
       else {
           return semid;
       }
    }
    else {
        return -1;
    }
}
 

int main() {
    int semid;
    struct sembuf buf;
    key_t key;

    buf.sem_num = 0; //acquire the first  semaphore from semaphore set.
    buf.sem_op = -1; //semaphore's semncnt must be greater than 1, then semop(semid, sembuf, 1) can success. In other word try acquire 1 from semaphore.
    buf.sem_flg = 0;
    
     
    key = ftok("sunriver", 'S');

    semid = initsem(key, 20); // try to initial a semaphore set whose has 20 semaphores 
    if (-1 == semid) { perror("initsem"); exit(-1);}

    printf("using getchar to control when to acquire semaphore start\n");
    getchar();
    printf("using getchar to control when to acquire semaphore finish\n");
 
    if ( -1 == semop(semid, &buf, 1)) {
         perror("acquire semaphore failed");
         exit(-1);
    }
    printf("acquire success\n");

    printf("using getchar to control when to release semaphore start\n");
    getchar();
    printf("using getchar to control when to release semaphore finished\n");

    buf.sem_op = 1;
    if ( -1 == semop(semid, &buf, 1)) {
        perror("release semaphore failed");
        exit(-1);
    }
    printf("release success\n");

    return 0;
}

     
