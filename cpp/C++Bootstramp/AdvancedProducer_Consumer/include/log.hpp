#ifndef _LOG_HPP
#define _LOG_HPP

#define LOG_ERROR 0
#define LOG_WARNNING 1
#define LOG_INFO 2
#define LOG_DEBUG 3

#include <pthread.h>
#include <string>




class Log {
public:
    void  log(int priority, const char* format, ...);

    void setMouduleName(const std::string& moduleName) {
        moduleName_ = moduleName;
    }

    std::string getModuleName() {
        return moduleName_;
    }

private:
    std::string moduleName_;
    static pthread_mutex_t mutex_;
};



#endif
