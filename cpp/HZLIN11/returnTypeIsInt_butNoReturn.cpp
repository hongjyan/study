#include <stdio.h>

int bar() {
    return 1;
}

int foo() {
    printf("test\n");
    bar();
}


int main() {
    if (foo()) {
	printf("here?\n");
    }
}
