#include <list>
#include <cstdio>
#include <pthread.h>
#include <unistd.h>

typedef std::list<int> LST;

void* threadFunc1(void* arg) {
    LST* lst = (LST*)arg;
    std::list<int>::iterator it = lst->begin();
    sleep(1);
    if (it != lst->end()) {
        printf("%d\n", *it);
    }
    return NULL;
}

void* threadFunc2(void* arg) {
    LST* lst = (LST*)arg;
    usleep(10000);
    lst->push_back(1);
    return NULL;
}     

int main() {
    std::list<int> lst;
    pthread_t pid1, pid2;
    pthread_create(&pid1, 0, threadFunc1, &lst);
    pthread_create(&pid2, 0, threadFunc2, &lst);
    return 0;
}
