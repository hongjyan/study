#include <stdio.h>
#include <iostream>


class cla {
public:
    cla() {};
    static int *p;
    static FILE* fd;
    ~cla() {};
    static void release();
};

int* cla::p = new int(1); //can
FILE* cla::fd = fopen("staticMember_dynamicMemory", "a+");
void cla::release() {
    if (NULL != p) { delete p; p = NULL; }
    if (NULL != fd) fclose(fd);
}

int main() {
    printf("*(cla::p) is %d\n", *cla::p);
    if (cla::fd) printf("file is opening\n");
    std::cin.ignore();
    cla::release();
    printf("cla::p is %p\n", cla::p);
}


