#include <stdio.h>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pfds[2];
    pipe(pfds);
    
    if (!fork()) {
        close(1);
        dup(pfds[1]);
        execlp("ls", "ls", NULL);
    } else {
        close(0);
        dup(pfds[0]);
        close(pfds[1]); //close pfds[1] in case result would print to pfds[1] instead of stdout
        execlp("wc", "wc", "-l", NULL);
    }
    
    wait(NULL);
    return 0;
}
