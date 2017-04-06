#include <cstdio>

struct Waiter {
    Waiter(const char* name): name_(name) {}
    static void showName(void* arg) { //callback function must be static, but this static want to show property of object, so add
                                      // arg.
         Waiter *p = (Waiter*)arg;
         p->showName();  
    }

    void showName() {
        printf("My name is %s\n", name_);
    }
private:
    const char* name_;
};

struct Waitress {
    Waitress(int age): age_(age) {}
    static void showAge(void* arg) {
        Waitress *p = (Waitress*)arg;
        p->showAge();
    }

    void showAge() {     
        printf("My age is %d\n", age_);
    }
private:
    const int age_;
};


typedef void Callback(void* owner);

struct Running {
    Running(): callbackOwner_(NULL), callback_(NULL) {}

    void run() {
        if (callback_ && callbackOwner_) callback_(callbackOwner_);
    }

    void setCallback(void* callbackOwner, Callback* callback) { 
        callbackOwner_ = callbackOwner;
        callback_ = callback;
    }
private:
    void* callbackOwner_;
    Callback* callback_;
};
        


int main()
{
    Running running;

    running.run(); //nothing
    Waiter waiter("xiaoming");
    running.setCallback(&waiter, Waiter::showName);
    running.run(); //My age is xiaoming.

    running.run();
    Waitress waitress(20);
    running.setCallback(&waitress, Waitress::showAge);
    running.run();

    return 0;
}
