#ifndef __APP_HPP__
#define __APP_HPP__

#include "testClient.hpp"
#include "testServer.hpp"

class App {
public:
    void start(int argc, char* argv[]);
private:
    void usage(char* argv[]);
};

#endif
