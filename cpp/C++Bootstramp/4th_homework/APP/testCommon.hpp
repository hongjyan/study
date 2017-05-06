#ifndef __TESTCOMMON_HPP__
#define __TESTCOMMON_HPP__
#include "TcpCnx.hpp"
#include <string>


class TestCommon {
public:
    TestCommon(int argc, char* argv[]);
    ~TestCommon();
    virtual void run()=0;


protected:
    void dealArgs(int argc, char* argv[]);
    void start(TcpCnx& tcpCnx);
    void static showRecv(const char* who, void *buf, unsigned int len);

private:
    void init();

protected:
    std::string localaddr_, remoteaddr_;
    unsigned short localport_, remoteport_;
};

#endif
