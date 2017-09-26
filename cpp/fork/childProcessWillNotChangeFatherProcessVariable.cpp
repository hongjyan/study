#include <unistd.h>
#include <stdio.h>
#include <unistd.h>


int main(){
    int i = 0;
    printf("Let's start\n");
    pid_t child = fork();
    if (!child) {
        i++;
    }
    usleep(1000); //sleep 1 s to let father process wait till "i++" executed in child process
    if (!child) printf("Father: i is %d after sleep, the address of i is %p\n", i, &i);  //i is 0 
    else printf("Child: i is %d after sleep, the address of i is %p\n", i, &i); // i is 1
    //but curious thing is that the address of i is the same between child process and father process
}
