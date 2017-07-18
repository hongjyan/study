#include <cstdio>
#include <stdexcept>
#include <pthread.h>

void* f(void* arg) {
    throw std::overflow_error("test");
    return NULL;
}


int main() {
    try {
        if (0 != pthread_create(0, 0, f, 0)) {
             printf("create thread failed\n");
        }
    } catch (std::overflow_error& e) {
        printf("%s\n", e.what());
    }
    return 0;
}
