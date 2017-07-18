#include "client.hpp"
#include "routerDealer.hpp"
#include <cstdio>
#include <cstring>

void usage(char* argv[])
{
    printf("start proxy by\n"
           "%s proxy\n\n\n",
           argv[0]);

    printf("start client by\n"
           "%s client --remoteaddr xxx --remoteport xxx.\n"
           "Alternative is \"--remoteaddr 127.0.0.1 --remoteport 12345\" or \"--remoteaddr 127.0.0.1 --remoteport 54321\"\n", 
           argv[0]);
}   

void showRecv(const char* who, void *buf, unsigned int len) {
    printf("%s<<<%.*s\r\n", who, len, (char*)buf);
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        usage(argv);
        return -1;
    }

    if (0 == strcasecmp(argv[1], "proxy")) {
        RouterDealer proxy;
        proxy.run();
    } else if (0 == strcasecmp(argv[1], "client")) {
        Client client;
        client.setRecvCallback(showRecv);
        client.run(argc, argv);
    } else {
        usage(argv);
        return -1;
    }

    return 0;
}
