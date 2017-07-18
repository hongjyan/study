#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    int stdin_copy = dup(0);
    int stdout_copy = dup(1);
    printf("1\n");
    close(0);
    close(1);
    printf("2 you can not see\n");

    int file1 = open("game", O_CREAT, S_IRWXU);
    int file2 = open("fun", O_CREAT, S_IRWXU);


    close(file1);
    close(file2);
    printf("3 you can not see\n");
    dup2(stdin_copy, 0);
    dup2(stdout_copy, 1);
    printf("4\n");
    close(stdin_copy);
    close(stdout_copy);
    printf("5\n");
}
