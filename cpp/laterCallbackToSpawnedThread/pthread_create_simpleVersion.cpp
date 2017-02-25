#include <pthread.h>
#include <cstdio>
#include <unistd.h>

class Lib {
public:
    Lib() {
        if (0 != pthread_create(&pid_, 0, threadFunc, 0)) {
            printf("Create thread failed\n");
        }
    }

    ~Lib() {}
private:
    pthread_t pid_;
    static void* threadFunc(void* arg) {
        while (true) {
            printf("I am alive\n");
            sleep(1);
        }
        return NULL;
    }
};

int main() {
    Lib lib;
    pthread_exit(NULL);
    return 0;
}
