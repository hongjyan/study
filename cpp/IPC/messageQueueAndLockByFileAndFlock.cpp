#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MSG_SIZE 1024

struct message {
    long mtype;
    char mtext[MSG_SIZE];
};

void child_process(int msgid, int lockfd) {
    struct flock lock;

    // 锁住消息队列
    lock.l_type = F_WRLCK;  // 该锁为写入锁
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    fcntl(lockfd, F_SETLKW, &lock); // 对lockfd设置锁状态。此处为设置为阻塞锁。阻塞锁是否上锁， 取决于lock的内容设置

    struct message msg;
    if (msgrcv(msgid, &msg, MSG_SIZE, 1, 0) == -1) {
        perror("msgrcv failed");
        exit(EXIT_FAILURE);
    }
    printf("Received message: %s", msg.mtext);

    // 解锁消息队列
    lock.l_type = F_UNLCK;
    fcntl(lockfd, F_SETLKW, &lock);
}

void parent_process(int msgid, int lockfd) {
    struct flock lock;

    // 锁住消息队列
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    fcntl(lockfd, F_SETLKW, &lock);

    struct message msg;
    printf("Enter message: ");
    fgets(msg.mtext, MSG_SIZE, stdin);
    msg.mtype = 1;
    if (msgsnd(msgid, &msg, strlen(msg.mtext)+1, 0) == -1) {
        perror("msgsnd failed");
        exit(EXIT_FAILURE);
    }
    printf("Message sent: %s", msg.mtext);

    // 解锁消息队列
    lock.l_type = F_UNLCK;
    fcntl(lockfd, F_SETLKW, &lock);
}

int main(int argc, char *argv[]) {
    int msgid;
    struct message msg;

    // 生成一个key，用于标识消息队列
    key_t key = ftok(".", 'm');
    if (key == -1) {
        perror("ftok failed");
        exit(EXIT_FAILURE);
    }

    // 创建消息队列
    msgid = msgget(key, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }

    // 创建锁文件
    int lockfd = open("lockfile", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (lockfd == -1) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        child_process(msgid, lockfd);
    } else {
        parent_process(msgid, lockfd);
    }

    // 删除消息队列
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("delete failed");
        exit(EXIT_FAILURE);
    }
}
