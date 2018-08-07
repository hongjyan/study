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
    int i = 0;
    printf("I am 1st\n");
    pid_t child = fork();
    printf("The address of child is %p\n", &child);
    printf("child is %d\n", child);
    printf("The address of i is %p\n", &i);
    printf("I am the 2nd, i is %d\n", i++);
    output(child);
    if (!child) {
        pid_t grandChild = fork();
        output(grandChild);
    }
    usleep(1000);
    printf("2nd time: The address of child is %p\n", &child);
    printf("2nd time: child is %d\n", child);
    
}
