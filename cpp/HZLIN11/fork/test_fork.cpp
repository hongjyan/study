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
    printf("Let's start\n");
    pid_t child = fork();
    printf("The address of child is %p\n", &child);
    printf("child is %d\n", child);
    printf("i is %d, The address of i is %p\n", i++, &i);
    output(child);
    if (!child) {
        pid_t grandChild = fork();
        output(grandChild);
        printf("i is %d in if, the address of i is %p\n", i++, &i);
    }
    usleep(1000);
    printf("i is %d after sleep, the address of i is %p\n", i, &i);
    printf("2nd time: The address of child is %p\n", &child);
    printf("2nd time: child is %d\n", child);
    
}
