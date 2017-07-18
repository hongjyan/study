#include "unp.h"
//version 1

/*
void sig_chld(int signo) {
    int stat;
    pid_t pid;
    if ((pid = wait(&stat)) < 0) 
	printf("wait fail\n");
    printf("child %d terminated\n", pid);
    return;
}
*/



//version 2
/*
void sig_chld(int signo) {
    int stat;
    pid_t pid;
    while ( (pid = waitpid(-1, &stat, 0)) > 0)  
	printf("child %d terminated\n", pid);
    return;
}
*/

    
//version 3
/*
void sig_chld(int signo) {
    printf("enter sig_chld\n");
    int stat;
    pid_t pid;
    while ( (pid = wait(&stat)) > 0) { 
	printf("child %d terminated\n", pid);
	sleep(1);
    }
    printf("exit sig_chld\n");
    return;
}
*/


/*
void sig_chld(int signo) {
    printf("enter sig_chld\n");
    int stat;
    pid_t pid;
    if ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
	printf("child %d terminated\n", pid);
    printf("exit sig_chld\n");
    return;
}
*/

//official version

void sig_chld(int signo) {
    printf("Enter sig_chld\n");
    int stat;
    pid_t pid;
    while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0) { 
	printf("child %d terminated\n", pid);
    }
    printf("Exit sig_chld\n");
    return;
}

    
    
