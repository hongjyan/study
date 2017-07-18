#include <stdio.h>
#include <fcntl.h>    //open, fcntl
#include <unistd.h>  //close, getpid, write.
#include <stdlib.h>  //exit
#include <sys/wait.h>
#include <cstring>

void* process(int fd, struct flock* fl) {
    
    fl->l_pid = getpid();
    fl->l_type = F_WRLCK;

    printf("use getchar() to control when to acquire file lock for pid:%d\n", getpid());
    char c;
    while ((c = getchar()) != '\n' && c != '\r' && c != EOF) {
        printf("input is %d in pid:%d\n", c, getpid());
    } //Use getchar in while, otherwise getchar will store \10(newline) into stdin buffer.
    printf("input is %d in pid:%d\n", c, getpid());




    if ((fcntl(fd, F_SETLKW, fl)) == -1) {
         perror("fcntl F_SETLKW for acquire");
         return NULL;
    }
    printf("PID:%d acquired lock done\n", getpid());

    char buf[300];
    memset(buf, 0, 300);
    snprintf(buf, 300,  "I am in PID:%d\n", getpid());
    write(fd, buf, 300);


    printf("use getchar() to control when to release file lock for pid:%d\n", getpid()); 
    while ((c = getchar()) != '\n' && c != '\r' && c != EOF) {                  
        printf("input is %d in pid:%d\n", c, getpid());
    } //Use getchar in while, otherwise getchar will store \10(newline) into stdin buffer.
    printf("input is %d in pid:%d\n", c, getpid());


   fl->l_type = F_UNLCK;

   if ((fcntl(fd, F_SETLK, fl)) == -1) {
       perror("fcntl F_SETLK for release");
       return NULL;
   }
   printf("PID:%d released lock done\n", getpid());

   close(fd);

   return fl;
}

int main() {
    int fd = open("sunriver", O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
    if (-1 == fd) { perror("open"); _exit(-1); }
 
    struct flock fl = {F_WRLCK, SEEK_SET, 0, 0, 0};
    printf("Parent PID is %d\n", getpid());
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed\n");
        close(fd);
	return 0;
    } else if (pid == 0) {
        printf("My pid is %d\n", getpid());
        process(fd, &fl);
        return 0;
    } else {
        printf("My pid is %d\n", getpid());
        process(fd, &fl);
        wait(NULL);
        return 0;
    }
}
