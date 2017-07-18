#include "lib.hpp"
#include <cstdio>

int Lib::start(int any, callback pcallback) {
    std::thread newthread(&Lib::threadFunc, this, any, pcallback);
    newthread.detach();
    return 0;
}

void Lib::threadFunc(int any, callback pcallback) {
    printf("any is %d\n", any); //just do something here.
    pcallback("xiaoli");
}

