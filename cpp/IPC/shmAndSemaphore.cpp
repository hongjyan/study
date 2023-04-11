#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <cstring>

using namespace std;

#define SHM_SIZE 1024
#define SEM_KEY 1234
#define SHM_KEY 5678

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int main() {
    int shmid, semid;
    char* shm_ptr;
    struct sembuf sem_wait = {0, -1, SEM_UNDO};
    struct sembuf sem_signal = {0, 1, SEM_UNDO};
    union semun sem_union;

    // 创建共享内存
    if ((shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        return -1;
    }

    // 挂载共享内存
    if ((shm_ptr = (char*) shmat(shmid, NULL, 0)) == (char*) -1) {
        perror("shmat");
        return -1;
    }

    // int semget(key_t key, int nsems, int semflg); 创建以key表示的semaphore set， 该set成员有nsems个。返回值是semaphore set的标识符。
    // 创建信号量
    if ((semid = semget(SEM_KEY, 1, IPC_CREAT | 0666)) < 0) {
        perror("semget");
        return -1;
    }

    // int semctl(int semid, int semnum, int cmd, ...); 对“信号量集合(由semid标识)里的第semnum个信号量” cmd操作， 根据cmd不同， 第四个参数取union semun(用户定义）里不同的成员。
    sem_union.val = 1;
    // 设置semid里的第0个semaphore的semval为sem_union.val即1. 目的是使信号量的初始值为1。 因为只有Semaphore.semval-sembuf.sem_op大于等于0， semaphare的P操作（即占用信号量，如上的sem_wait）才不会阻塞。
    if (semctl(semid, 0, SETVAL, sem_union) < 0) {
        perror("semctl");
        return -1;
    }

    // 创建子进程
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return -1;
    } else if (pid == 0) {
        // 子进程写入共享内存
        const char* message = "Hello from child process";
        semop(semid, &sem_wait, 1); // 请求临界区
        strncpy(shm_ptr, message, strlen(message));
        semop(semid, &sem_signal, 1); // 释放临界区。 释放信号量永远不会阻塞。
        exit(0);
    } else {
        // 等待子进程结束
        wait(NULL);
        cout << "Parent process received message: " << shm_ptr << endl;
    }

    // 删除信号量和共享内存
    semctl(semid, 0, IPC_RMID);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
