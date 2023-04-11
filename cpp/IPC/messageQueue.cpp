#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>

#define MSG_SIZE 1024

struct message {
    long mtype;
    char mtext[MSG_SIZE];
};

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
    

    msg.mtype = 1;
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    // 子进程发送消息
    if (0 == pid) {
        printf("Enter message: ");
        fgets(msg.mtext, MSG_SIZE, stdin);
        if (msgsnd(msgid, &msg, strlen(msg.mtext)+1, 0) == -1) {
            perror("msgsnd failed");
            exit(EXIT_FAILURE);
        }
        printf("Message sent: %s", msg.mtext);
    } 
    // 父进程接收消息
    else {
        if (msgrcv(msgid, &msg, MSG_SIZE, msg.mtype, 0) == -1) {
            perror("msgrcv failed");
            exit(EXIT_FAILURE);
        }
        printf("Received message: %s", msg.mtext);

        // 删除消息队列
        if (msgctl(msgid, IPC_RMID, NULL) == -1) {
            perror("msgctl failed");
            exit(EXIT_FAILURE);
        }

        wait(NULL);
        exit(EXIT_SUCCESS);
    }
    
    return 0;
}
