#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MSG_SIZE 256

int main() {
    int pipefd[2];
    pid_t pid;
    char msg[MSG_SIZE];

    // 创建管道
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // 创建子进程
    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // 子进程
        close(pipefd[1]); // 关闭写端
        while (read(pipefd[0], msg, MSG_SIZE) > 0) {
            printf("Child received: %s", msg);
        }
        close(pipefd[0]); // 关闭读端
        exit(EXIT_SUCCESS);
    } else {
        // 父进程
        close(pipefd[0]); // 关闭读端
        sprintf(msg, "Hello from parent!\n");
        write(pipefd[1], msg, strlen(msg));
        close(pipefd[1]); // 关闭写端
        wait(NULL); // 等待子进程结束
        exit(EXIT_SUCCESS);
    }

    return 0;
}
