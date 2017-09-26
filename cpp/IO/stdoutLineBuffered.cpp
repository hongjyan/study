#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    //close and open stdout once to make "setvbuf() function may only be used after opening a stream and before any other operations have been performed on  it"
    int stdout_cp = dup(1);
    close(1);
    dup(stdout_cp);

    setvbuf(stdout, NULL, _IOFBF, 1024);  //seems _IOFBF take effect
    putchar('a');
    putchar('b');
    putchar('c');
    putchar('d');
    getchar();
//    putchar('\n');
    setvbuf(stdout, NULL, _IOLBF, 1024);  //but _IOLBF can not take effect
    putchar('a');
    putchar('b');
    putchar('c');
    putchar('d');
    getchar();
}
