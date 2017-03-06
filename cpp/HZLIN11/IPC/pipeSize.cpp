#include <unistd.h>
#include <stdio.h>

int main() {
    int tube[2];
    char c = 'c';
    
    if (-1 == pipe(tube)) { perror("pipe"); _exit(-1); }
    
    char buf[30];
    write(tube[1], "test", 5);
    read(tube[0], buf, 30);
    printf("read from tube[1] is: %s\n", buf);
  
    for(int i= 0;;i++) {
	fprintf(stdout, "%d bytes written\n", i+1);
	if ( -1 == write(tube[1], &c, 1)) {
	    perror("write");
	    _exit(-1);
	}
    }

    return 0;
}
