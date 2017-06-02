#ifndef __APP_HPP__
#define __APP_HPP__

#include "TcpApp/TcpAppClient.hpp"
#include "TcpApp/TcpAppServer.hpp"
#include "ZMQApp/ZMQAppClient.hpp"
#include "ZMQApp/ZMQAppServer.hpp"

class App {
public:
    void start(int argc, char* argv[]);
private:
    void usage(char* argv[]);
};

#endif
