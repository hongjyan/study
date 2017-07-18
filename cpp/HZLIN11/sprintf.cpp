#include <stdio.h>
int main(int argc, char* argv[]) {
    int i = 0;
    char s[3];
    sprintf(s,"%d", 120);
    while (s[i]) {
        printf("%c", s[i++]);
    }
    printf("\n");
}
