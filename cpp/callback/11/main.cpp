#include <cstdio>
#include <thread>
#include <functional>
#include <unistd.h>

enum class Event {
    one,
    two,
    three
};

using Callback = std::function<void (Event)>;

struct Running {
    Running(): running_(false), callback_(nullptr) {}

    void run() {
        printf("running: do some preperation....\n");
        //std::thread can take multiType parameter.
        //must using running::startThread. startThread will cause error:ISO C++ forbids taking the address of an unqualified or parenthesized non-static member function to form a pointer to member function.
        //non-static methond can be run in a seperated thread compared than C++98
        std::thread eventThread(&Running::startThread, this, 1);
        eventThread.detach();
    }

    void startThread(int threadNum) {      
        printf("I am event thread:%d\n", threadNum);
        while (running_) {
            if (nullptr != callback_) {
                callback_(Event::two);
            } else {
                printf("Callback function is not set\n");
            }
            sleep(1);
        }
    }
        
    void start() {
        running_ = true;
    }

    void stop() {
        running_ = false;
    }

    void setCallback(Callback callback) {
        callback_ = callback;
    }
private:
    bool running_;
    Callback callback_;
};

struct User {
    void eventNotifyCallback(Event event) {
        printf("User: event is %d\n", (int)event);
    }
};


int main() {
    User user;
    Running running;
    running.start();
    running.run();
    sleep(2);
    running.setCallback(std::bind(&User::eventNotifyCallback, user, std::placeholders::_1)); //callback function van be non-static in C++11.
    sleep(10);
    running.stop();
    return 0;
}

