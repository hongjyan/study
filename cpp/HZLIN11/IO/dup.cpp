#include <unistd.h>
#include <stdio.h>

int main() {
    int stdin_cp = dup(0);
    int stdout_cp = dup(2);

    printf("The stdin_cp is %d\n", stdin_cp); //proe "dup() uses the lowest-numbered unused descriptor for the new descriptor."
    printf("The stdout_cp is %d\n", stdout_cp);


    close(0);
    close(1);

    char s[200];
    fprintf(fdopen(stdout_cp, "w"), "please input something\n");
    fgets(s, 200, fdopen(stdin_cp, "r"));
    fprintf(fdopen(stdout_cp, "w"), "you input is %s\n", s);

    printf("this line won't show\n");

    dup(stdin_cp);
    dup(stdout_cp);

    printf("this line should show\n");

    close(stdin_cp);
    close(stdout_cp);


}
