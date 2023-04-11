#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pfds[2];

    pipe(pfds);
    printf("START---pfds[0] is %d, pfds[1] is %d\n", pfds[0], pfds[1]);

    // 虽然管道是单向通信，即fd[0]用作读， fd[1]用作写。但父与子进程均可以读写。所以理论可以双向通信。
    // 但这样比较混乱，不知道谁读谁写。
    // 所以一般要么父进程读，子进程写。 要么父进程写，子进程读。即父子要分别关闭不同端。
    if (!fork()) { //child process
        dup2(pfds[1], STDOUT_FILENO); // 标准输出重定向至pfd[1]（写段）
        close(pfds[0]);
        execlp("ls", "ls", NULL); // ls的内容将写入pfd[1]
    }
    else { //parent process
        dup2(pfds[0], STDIN_FILENO); //标准输入重定向到pfds[0]， 所以标准输出的内容将被pfd[0]读到。
        close(pfds[1]);  // 子进程结束时， 会关闭所有管道，但如果父进程不关闭这个管道得话， 那么程序就block在读中
        execlp("wc", "wc", "-l", NULL);
    }
    wait(NULL);
    return 0;
} 
