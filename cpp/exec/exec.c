#include <unistd.h>
#include <stdio.h>

int main() {
    pid_t pid = 0;

    pid = fork();
    if (pid == 0) { //fork child process to run execlp, otherwise execlp will replace parent process which will lead main program exit
        //int execlp(const char *file, const char *arg, ...);
        //p means found file from PATH if the specified filename does not contain a  slash  (/) 
        execlp("ping", "myping", "-c 1", "127.0.0.1", NULL);
    }
    
    pid = fork(); //fork another to test execl
    if (pid == 0) {
        //int execl(const char *path, const char *arg, ...);
        //must specify whole path, otherwise e.g. ls won't be found.
        execl("/usr/bin/ls", "ll", "-l", "-a", NULL);
    }

    //test execvp
    pid = fork();
    if ( pid == 0) {
        char *const myargv[] = {"lla", "-l", "-a", NULL};
        //int execvp(const char *file, char *const argv[]);
        execvp("ls", myargv);
    }  
    pid = fork();
    if ( pid == 0) {
        printf("the second execvp\n");
        //output is that same with: ls "-l -a"
        char *const myargv[] = {"lls", "-l -a", NULL};
        //int execvp(const char *file, char *const argv[]);
        execvp("ls", myargv);
    } 
    return 0;
}
