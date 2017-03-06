#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>

int main()
{
    int stdin_copy, stdout_copy;
    
    stdin_copy = dup(0);
    printf("stdin_copy is %d\n", stdin_copy);

    int file1 = open("game", O_CREAT, S_IRWXU); 
    printf("file1 is %d\n", file1);

    int res = dup2(STDOUT_FILENO, file1);
    printf("res is %d\n", res);
    const char *p = "have fun!!!\n";
    write(file1, p, strlen(p)+1); //no "have fun!!!" in file1, since file1 is the copy of STDOUT.
}
  

 
    
