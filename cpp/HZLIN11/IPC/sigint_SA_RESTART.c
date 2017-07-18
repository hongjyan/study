#include <cstdio>
#include <signal.h>
#include <cstdlib>


void sigint_handler(int sig) {
    printf("catch SIGINT\n");
}


int main() {
    char msg[100];
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sa.sa_flags = SA_RESTART; 
    sigemptyset(&sa.sa_mask);

    if (-1 == sigaction(SIGINT, &sa, 0)) {
        perror("sigaction");
        exit(-1);
    }

        printf("Enter sometine\n");
        if (NULL == fgets(msg, 100, stdin)) { //fgets is a system call, if it is interupted by SIGINT, it will return an error.
                                              //So, will go to this if.
            perror("fgets");
        }
        else {
            printf("you entered: %s\n", msg);
        }

    return 0;
}
    
     
