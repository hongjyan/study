#include "App.hpp"
#include <cstring>
#include <unistd.h>

void App::start(int argc, char* argv[])
{
    if (argc < 4) {
        usage(argv);
        _exit(-1);
    }

    if (0==strcmp(argv[1], "tcpserver")) {
        TcpAppServer tcpAppServer(argc, argv);
        tcpAppServer.run();
    } else if (0==strcmp(argv[1], "tcpclient")) {

        TcpAppClient tcpAppClient(argc, argv);
        tcpAppClient.run();
    } else if (0==strcmp(argv[1], "zmqserver")) {

        ZMQAppServer zmqAppServer(argc, argv);
        zmqAppServer.run();
    } else if (0==strcmp(argv[1], "zmqclient")) {

        ZMQAppClient zmqAppClient(argc, argv);
        zmqAppClient.run();
    } else {
        printf("Wrong parameter, exit\n");
        _exit(-1);
    }
}

void App::usage(char* argv[]) {
    printf("start tcpserver by\n"
            "%s tcpserver --localaddr xxx --localport xxx --remoteaddr xxx --remoteport xxx\n"
            "--localaddr, --localport is mandatory\n",
            argv[0]);
    printf("\n\n");
    printf("start tcpclient by\n"
            "%s tcpclient --localaddr xxx --localport xxx --remoteaddr xxx --remoteport xxx\n"
            "--remoteaddr, --remoteport is mandatory\n",
            argv[0]);
    printf("\n\n");
    printf("start zmqserver by\n"
            "%s zmqserver --localaddr xxx --localport xxx --remoteaddr xxx --remoteport xxx\n"
            "--remoteaddr, --remoteport is mandatory\n",
            argv[0]);
    printf("\n\n");
    printf("start zmqclient by\n"
            "%s zmqclient --localaddr xxx --localport xxx --remoteaddr xxx --remoteport xxx\n"
            "--remoteaddr, --remoteport is mandatory\n",
            argv[0]);
}



int main(int argc, char* argv[]) {
    App app;
    app.start(argc, argv);
#ifdef DEBUG
    printf("------------------------return main()---------------\n");
#endif
    return 0;
}
