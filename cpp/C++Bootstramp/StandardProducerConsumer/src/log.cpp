#include "log.hpp"
#include <exception>
#include <stdio.h>
#include <stdarg.h>

pthread_mutex_t Log::mutex_ = PTHREAD_MUTEX_INITIALIZER;

void Log::log(int priority, const char* format, ...)
{
    std::string msgFinal;
    msgFinal = "[" + moduleName_ + "    ";

    va_list vargs;
    char msg[1000];
    va_start(vargs, format);
    vsnprintf(msg, 1000, format, vargs);
    msg[999] = '\0';


    switch (priority) {
    case 0:
        msgFinal += "ERROR]: ";
        break;
    case 1:
        msgFinal += "WARNNING]: ";
        break;
    case 2:
        msgFinal += "INFO]: ";
        break;
    case 3:
        msgFinal += "DEBUG]: ";
        break;
    default:
        msgFinal += "UNKNOWNLEVEL]: ";
        break;
    }

    msgFinal += msg;

    if (pthread_mutex_lock(&mutex_)) {
        throw std::exception();
    }
    else {
        printf("%s", msgFinal.c_str());
    }

    if (pthread_mutex_unlock(&mutex_)) {
        throw std::exception();
    }
}
