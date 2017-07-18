#include <stdio.h>
#include <exception>

int main() {
    std::exception e("time is a magic");
    printf("e.what is %s", e.what());
} 
