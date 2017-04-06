#include <cstdio>
#include <unistd.h>
#include <pthread.h>
 
enum Event{
    zero,
    one,
    two,
    three
};

typedef void (*Callback)(Event);


struct Running {
    Running(): pCallback_(NULL), running_(false) {}

    void run() {
        printf("do some preparation....\n");
        startThread();
    }

    void startThread() {
        pthread_create(&pid, NULL, Running::callbackThread, this); //the function needed to be run in pthread_create should be static
    }

    static void* callbackThread(void* arg) {   //cast arg to Running to use the non-static property of arg-object.
        Running *p = (Running*)arg;
        p->callbackThread();
        pthread_exit(NULL);
    }


    void callbackThread() {
        while (running_) {
            if (pCallback_) { 
                pCallback_(zero);
                sleep(1);
            } else {
               printf("callback is null\n");
               sleep(5);
            }
        }
    }

    void setCallback(Callback pCallback) {
        pCallback_ = pCallback;
    }

    void start() {
        running_ = true;
    }

    void stop() {
        running_ = false;
        pthread_join(pid, NULL);
    }
private:
    Callback pCallback_;
    bool running_;
    pthread_t pid;
};
    

struct User {
    void run() {
        printf("User do something here...\n");
        Running running;
        running.start();
        running.run();
        sleep(2);
        running.setCallback(eventNotifyCallback);
        sleep(10);
        running.stop();
    }
        
    static void eventNotifyCallback(Event event) {
        printf("Event is %d\n", event);
    }
};



int main() {
    User user;
    user.run();
    pthread_exit(NULL);
    return 0;
}
