#include "ICnx.hpp"


ICnx::ICnx()
{
    stopThread_ = false;
    callback_ = nullptr;
}

ICnx::~ICnx()
{
}

bool ICnx::startThread() {
    int r;
    r = pthread_create(&tidDataThread, NULL, ICnx::recvThread, this);
    if (0 == r) {
        return true;
    } else {
        errno = r;
        printf("Create thread failed, reason:%s\n", strerror(errno));
        return false;
    }
}


void ICnx::stopThread() {
    stopThread_ = true;
    pthread_join(tidDataThread, NULL);
}

void* ICnx::recvThread(void* arg) {
    if (NULL == arg) return NULL;

    ICnx *p = static_cast<ICnx*>(arg);

    p->recvThread();
    pthread_exit(NULL);
}


void ICnx::setCallback(Callback callback) {
    callback_ = callback;
}
