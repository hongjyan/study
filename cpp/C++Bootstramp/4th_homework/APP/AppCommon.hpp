#ifndef __TESTCOMMON_HPP__
#define __TESTCOMMON_HPP__
#include "ICnx.hpp"
#include <string>


class AppCommon {
public:
    AppCommon(int argc, char* argv[]);
    ~AppCommon();
    virtual void run()=0;


protected:
    void dealArgs(int argc, char* argv[]);
    void start(ICnx& cnx);
    void static showRecv(const char* who, void *buf, unsigned int len);

private:
    void init();

protected:
    std::string localaddr_, remoteaddr_;
    unsigned short localport_, remoteport_;
};

#endif
