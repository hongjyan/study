#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pfds[2];

    pipe(pfds);
    printf("START---pfds[0] is %d, pfds[1] is %d\n", pfds[0], pfds[1]);

    if (!fork()) { //child process
        dup2(pfds[1], STDOUT_FILENO);
        execlp("ls", "ls", NULL);
   
    }
    else { //parent process
        dup2(pfds[0], STDIN_FILENO);
        close(pfds[1]);  //will print to pfds[1] instend of STDOUT if not close pfds[1]. Which cause ternimal always is in "input state", reason unknown.
        execlp("wc", "wc", "-l", NULL);
    }
    wait(NULL);
    return 0;
} 
