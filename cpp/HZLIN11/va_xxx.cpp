//2016.1.4
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <execinfo.h>

void common_printf(char* format, ...) {
    va_list args;  //use this args to take variable arguments. But still need specify args's output format
    char buffer[1000];
    FILE *fp = NULL;

    va_start(args, format); //initializes args to retrieve the additional arguments after formats. In this example, args was initialized as "game", 1, 2, 3
    vsprintf(buffer, format, args); //write formatted data(format specify) from variable argument(args) to string(buffer).
    printf(buffer);
    va_end(args); //counterparts of va_start like: malloc&free
    fp = fopen("/home/hongjyan/cpp/va_xxx.txt", "a+");
    if (NULL != fp) {
        fseek(fp, SEEK_SET, SEEK_END);
        fputs(buffer, fp);
        fclose(fp);
    }
}



void show_stackframe() {
    #define SIZE 100
    void *buffer[SIZE];
    char **strings;
    int npos = 0;
    
    npos = backtrace(buffer, SIZE);
    strings = backtrace_symbols(buffer, npos);
    if (strings == NULL) {
        common_printf(NULL, "Error happens in JsiTwConnection::show_stackframe");
    }
    
    for (int j=0; j<npos; j++) {
        common_printf("%s", strings[j]);
        common_printf("\r\n");
    }
    
    free(strings);
}  

void f1() {
    show_stackframe();
}

void f2() {
    f1();
}

void f3() {
    f2();
}

int main() {
    common_printf("TWLog: %s-%d-%d-%d\n", "game", 1, 2, 3);
    f3(); 
    return 0;
}
