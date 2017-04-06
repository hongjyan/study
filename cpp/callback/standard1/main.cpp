#include <cstdio>
#include <unistd.h>
 
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
        printf("do something....\n");
        startThread();
    }

    void startThread() {
        pthread_create(NULL, NULL, callbackThread, this); //the function needed to be run in pthread_create should be static
    }

    static void* callbackThread(void* arg) {   //cast arg to Running to use the non-static property of arg-object.
        Running *p = (Running*)arg;
        p->callbackThread();
        pthread_exit(NULL);
    }


    void callbackThread() {
        while (running_) {
            if (pCallback_) { 
                pCallback_(zero)
                sleep(1);
            } else {
               printf("callback is null\n");
               sleep(5);
            }
        }
    }

    void setCallback(Callback* pCallback) {
        pCallback_ = pCallback;
    }

    void start() {
        running_ = true;
    }

    void stop() {
        running_ = false;
    }
private:
    Callback* pCallback_;
    bool running_;
};
    

struct User {
    void start() {
        printf("do something here...\n");
        Running running;
        running.start();
        sleep(2);
        running.setCallback(eventNotifyCallback);
    }
        
    static void eventNotifyCallback(Event event) {
        event_ = event;
        printf("Evenet is %d, do something....\n", event_);
    }
};



int main() {
    User user;
    User.start();
    sleep(10);
    User.stop();
    pthread_exit(NULL);
    return 0;
}
