#include <cstdio>
#include <signal.h>
#include <cstdlib>
#include <unistd.h>

volatile sig_atomic_t got_usr1;

void sigusr1_handler(int sig) {
    got_usr1 = 1;
}


int main() {
    struct sigaction sa;
    sa.sa_handler = sigusr1_handler;
    sa.sa_flags = 0; 
    sigemptyset(&sa.sa_mask);

    if (-1 == sigaction(SIGUSR1, &sa, 0)) {
        perror("sigaction");
        exit(-1);
    }
    
    while (!got_usr1) {
        printf("process %d is working hard\n", getpid());
        sleep(1);
    }

    printf("got_usr became 1, signal USR1 was caught\n");

    return 0;
}
    
     
