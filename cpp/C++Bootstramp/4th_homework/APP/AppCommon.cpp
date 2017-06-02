#include "AppCommon.hpp"
#include <unistd.h>
#include <getopt.h>
#include <iostream>
#include <exception>


AppCommon::AppCommon(int argc, char* argv[])
{
    init();
    dealArgs(argc, argv);
}

void AppCommon::init()
{
    localport_ = 0;
    remoteport_ = 0;
}

AppCommon::~AppCommon() {}

void AppCommon::dealArgs(int argc, char* argv[]) {
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

void AppCommon::showRecv(const char* who, void *buf, unsigned int len) {
    printf("%s<<<%.*s\r\n", who, len, (char*)buf);
}

void AppCommon::start(ICnx& cnx) {
    cnx.setCallback(showRecv);
    bool bRet = cnx.start(localaddr_.c_str(), localport_, remoteaddr_.c_str(), remoteport_);
    if (!bRet) {
        printf("start cnx_ failed, exit\n");
        _exit(-1);
    }

    printf("start dialogue\n");
    std::string line;

    if (bRet) {
        while (std::getline(std::cin, line)) {
            unsigned int len = line.length();
            int sent = cnx.send(line.c_str(), len, 7500);
            if (-1 == sent || 0 == sent) {
                printf("Send data failed, break.\n");
                break;
            } else if (len != (unsigned int)sent) {
                printf("Send partial data, bearing\n");
            }
        }
    }

    cnx.stop();
}


