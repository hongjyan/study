#include <unistd.h>
#include <stdio.h>
#include <unistd.h>

void output(pid_t pid) {
    if(pid != 0){
        printf("I am parent process. My PID is %d, my parent PID is %d\n", getpid(), getppid());
    }
    else {
        printf("I am child process. My PID is %d, my parent PID is %d\n", getpid(), getppid());
    }
}

int main(){
    int i=2;
    while (i>0) {
        pid_t pid = fork();
        if (pid > 0) {
            i--;
            printf("i is %d\n", i);
            printf("pid is larger than 0, my own pid is %d, my parent pid is %d, my son pid is %d\n", getpid(), getppid(), pid);
        }
        else if (pid == 0) {
            printf("pid is %d\n", pid);
            break;
        }
        else {
            break;
        }
    }

}
