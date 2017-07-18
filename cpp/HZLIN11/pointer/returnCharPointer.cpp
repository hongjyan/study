#include <cstdio>

const char* foo() {
    char arr[] = {'a', 'b', 'c', '\0'};
    return arr;
}

int main() {
    const char* p = foo();
    printf("%s\n", p);
}
