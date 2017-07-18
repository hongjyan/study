#include <list>
#include <cstdio>
#include <pthread.h>
#include <unistd.h>

typedef std::list<int> LST;
 
void* threadFunc1(void* arg) {
    LST* lst = (LST*)arg;
    sleep(2);  //as expected if sleep here
    std::list<int>::iterator it = lst->begin();
    //sleep(2); //none output if sleep here
    if (it != lst->end()) {
        printf("%d\n", *it);
    }
    return NULL;
}

void* threadFunc2(void* arg) {
    LST* lst = (LST*)arg;
    lst->push_back(1);
    return NULL;
}     

int main() {
    std::list<int> lst;
    pthread_t tid1, tid2;
    pthread_create(&tid1, 0, threadFunc1, &lst);
    pthread_create(&tid2, 0, threadFunc2, &lst);
    pthread_exit(NULL);
    return 0;
}
