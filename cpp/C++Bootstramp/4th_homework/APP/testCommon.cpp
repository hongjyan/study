#include "testCommon.hpp"
#include <unistd.h>
#include <getopt.h>
#include <iostream>
#include <exception>


TestCommon::TestCommon(int argc, char* argv[])
{
    init();
    dealArgs(argc, argv);
}

void TestCommon::init()
{
    localport_ = 0;
    remoteport_ = 0;
}

TestCommon::~TestCommon() {}

void TestCommon::dealArgs(int argc, char* argv[]) {
#ifdef DEBUG
    for (int i=0; i!=argc; i++) {
        printf("%s\n", argv[i]);
    }
#endif
    const char *optstring = "a:b:c:d:";
    const struct option long_options[] = {
            {"localaddr", required_argument, 0, 'a'},
            {"remoteaddr", required_argument, 0, 'b'},
            {"localport", required_argument, 0, 'c'},
            {"remoteport", required_argument, 0, 'd'},
            {0, 0, 0, 0}
    };

    int option = 0;
    while (-1 !=  (option = getopt_long(argc, argv, optstring, long_options, 0)) ) {
        switch (option) {
        case 'a':
            localaddr_ = optarg;

            break;
        case 'b':
            remoteaddr_ = optarg;
            break;
        case 'c':
            try {
                localport_ = std::stoul(optarg);
            } catch (std::exception& e) {
                printf("invalid value:%s for \"localport\"\n", optarg);
                _exit(-1);
            }
            break;
        case 'd':
            try {
                remoteport_ = std::stoul(optarg);
            } catch (std::exception& e) {
                printf("invalid value:%s for \"remoteport\"\n", optarg);
                _exit(-1);
            }
            break;
        case '?':
            printf("unknown option\n");
            break;
        default:
            printf("?? getopt returned character code 0%o ??\n", option);
        }
    }
}

void TestCommon::showRecv(const char* who, void *buf, unsigned int len) {
    printf("%s<<<%.*s\n", who, len, (char*)buf);
}

void TestCommon::start(TcpCnx& tcpCnx) {
    tcpCnx.setCallback(showRecv);
    bool bRet = tcpCnx.start(localaddr_.c_str(), localport_, remoteaddr_.c_str(), remoteport_);
    if (!bRet) {
        printf("start tcpCnx_ failed, exit\n");
        _exit(-1);
    }

    printf("start dialogue\n");
    std::string line;

    if (bRet) {
        while (printf(">>>"), std::getline(std::cin, line)) {
            tcpCnx.send(line.c_str(), line.length(), 7500);
        }
    }

    tcpCnx.stop();
}


