#include "App.hpp"
#include <cstring>
#include <unistd.h>

void App::start(int argc, char* argv[])
{
    if (argc < 4) {
        usage(argv);
        _exit(-1);
    }

    if (0==strcmp(argv[1], "server")) {
        TestServer testServer(argc, argv);
        testServer.run();
    } else if (0==strcmp(argv[1], "client")) {

        TestClient testClient(argc, argv);
        testClient.run();
    } else {
        printf("Wrong parameter, exit\n");
        _exit(-1);
    }
}

void App::usage(char* argv[]) {
    printf("start server by\n"
            "%s server --localaddr xxx --localport xxx --remoteaddr xxx --remoteport xxx\n"
            "--localaddr, --localport is mandatory\n",
            argv[0]);
    printf("\n\n");
    printf("start client by\n"
            "%s client --localaddr xxx --localport xxx --remoteaddr xxx --remoteport xxx\n"
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
