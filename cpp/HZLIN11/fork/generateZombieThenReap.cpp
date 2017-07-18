#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>

int main(){
//    signal(SIGCHLD, SIG_IGN); //if any current process receive SIGCHLD from any process, treat it as SIG_IGN. then the process who signal SIGCHLD can be reaped also. Don't know reason. 
    pid_t pids[10];
    int i;
    for (i = 9; i >= 0; i--) {
        pids[i] = fork();
        if (pids[i] == 0) {
            sleep(i+1);
            _exit(0);
        }
     }

     //parent process will stay in for loop below for while till the last process end, meanwhile some previous process ended already. so all those child process will stay in zombie state for long or soon.
     for (i = 9; i >= 0; i--) {
          waitpid(pids[i], NULL, 0); //wait here till pid[i] terminate.
//          waitpid(pids[i], NULL, WNOHANG); //WNOHANG: return immediately if no child has exited. means parent process will quit quickly, then child process will be reparent to PID:1. And PID:1 will reap process periodically, then there won't be zombie process. 
     }
     wait(0);
     return 0;
}
