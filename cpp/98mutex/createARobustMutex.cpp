#include <pthread.h>
#include <cstdio>
#include <string.h>


int main() {
    pthread_mutexattr_t attr;
    pthread_mutex_t mutex;

    if (pthread_mutexattr_init(&attr) || pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_ROBUST)) {
        printf("set mutex robust attribute failed, exit\n");
        return 0;
    }


    if (pthread_mutex_init(&mutex, &attr)) {
        printf("init mutex failed.\n");
        return 0;
    }


    printf("init a robust mutex success\n");
    return 0;
}
