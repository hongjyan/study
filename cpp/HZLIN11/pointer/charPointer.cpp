#include <stdio.h>
#include <stdlib.h> // for free

int main() {
    char* line = NULL;
    
    ssize_t read = 0;
    size_t  n = 0;
    printf("input a line please\n");
    if ((read = getline(&line, &n, stdin) != -1)) {
        printf("line is %s\n", line);
        printf("&line is %p\n", &line);
    }

    if (line) free(line);
}
         
