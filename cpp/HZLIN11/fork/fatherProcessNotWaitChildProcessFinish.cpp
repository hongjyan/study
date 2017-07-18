#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>

int main(){
    printf("Let's start\n");
    pid_t child = fork();
    if (!child) {
         sleep(6);
         printf("Finished in child\n");
	 _exit(0);
    }
    printf("Finished\n");
//    wait(0);  //father process will terminate immediatelly if won't wait here. Then child process will be zombie process. But child process will be reparent to PID:1 which is "init", init process will preiodically reap any zombies by execute system call "wait".
}
