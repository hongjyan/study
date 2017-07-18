#include "client.hpp"

Client::Client()
{
    remoteaddr_ = "";
    remoteport_ = "";
    bStop_ = false;
    pRecvCallback_ = nullptr;
    ctx_ = zmq_ctx_new();
    zmqSocket_ = zmq_socket(ctx_, ZMQ_DEALER);
}

Client::~Client() 
{
    pRecvCallback_ = nullptr;
    if (zmqSocket_) {
         zmq_close(zmqSocket_);
         zmqSocket_ = nullptr;
    }

    if (ctx_) {
        zmq_ctx_destroy(ctx_);
        ctx_ = nullptr;
    }
}

void Client::dealArgs(int argc, char* argv[])
{
    const char *optstring = "a:p:";
    const struct option long_options[] = {
        {"remoteaddr", required_argument, 0, 'a'},
        {"remoteport", required_argument, 0, 'p'},
        {0, 0, 0, 0}
    };

    int option = 0;
    while (-1 != (option = getopt_long(argc, argv, optstring, long_options, 0)) ) {
        switch (option) {
        case 'a':
            remoteaddr_ = optarg;
            break;
        case 'p':
            remoteport_ = optarg;
            break;
        case '?':
            printf("unknown option\n");
            break;
        default:
            printf("?? getopt returned character code 0%o ??\n", option);
        }
    }

    if (remoteaddr_.empty() || remoteport_.empty()) {
        printf("Missing \"--remoteaddr xxx\" or \"--remoteport xxx\", exit\n"); 
        _exit(-1);
    }
}

void Client::run(int argc, char* argv[])
{
    dealArgs(argc, argv);
    char identity[100] = {0};
    snprintf(identity, 100, "%s:%s", remoteaddr_.c_str(), remoteport_.c_str());
    if (-1 == zmq_setsockopt(zmqSocket_, ZMQ_IDENTITY, &identity, strlen(identity)+1)) {
        printf("zmq_setsockeopt failed\n");
        _exit(-1);
    }

    if (!start()) {
        _exit(-1);
    }
    
    std::string line;
    //std::string remote = remoteaddr_ + ":" + remoteport_;
    while (std::getline(std::cin, line)) {
        //send(remote.c_str(), remote.length(), ZMQ_SNDMORE);
        unsigned int len = line.length();
        int sent = send(line.c_str(), len, 0);
        if (-1 == sent) {
            printf("Send data failed, break.\n");
            break;
        } else if (len != (unsigned int)sent) {
            printf("Send partial data, bearing\n");
        }
    }

    stop();
}

bool Client::start()
{
    std::ostringstream oss;
    oss << "tcp://" << remoteaddr_ << ":" << remoteport_;
    if (0 != zmq_connect(zmqSocket_, oss.str().c_str())) {
        printf("Connect client to %s failed causes starting client failed\n", oss.str().c_str());
        return false;
    }   

    recvThread_ = std::thread(&Client::recvDaemon, this);
    recvThread_.detach();

    printf("Start client success\n");
    return true;
}

void Client::stop()
{
    bStop_ = true;
}

void Client::setRecvCallback(pCallback pRecvCallback)
{
    pRecvCallback_ = pRecvCallback;
}

void Client::recvDaemon()
{
#ifdef DEBUG
    printf("enter RecvDaemon\n");
#endif
    char identity[100] = {0};
    char content[MAXRECV-100];
    int recvmore = 1;
    size_t smore = sizeof(recvmore);
    int identityRecvd = 0, contentRecvd = 0, recvd = 0;

    while (true && !bStop_) {
        memset(identity, 0, 100);
        memset(content, 0, MAXRECV-100);
        identityRecvd = recv(identity, 100);
        if (0 >= identityRecvd) {
            printf("recv identity failed, break\n");
            break;
        }

#ifdef DEBUG
            printf("identity is: %s\n", identity);
#endif
        recvmore = 1;
        while (recvmore) {
            if (-1 == zmq_getsockopt(zmqSocket_, ZMQ_RCVMORE, &recvmore, &smore)) {
                printf("zmq_getsockopt failed, break\n");
                break;
            }
            if (recvmore) {
                recvd = recv(content, MAXRECV-100-contentRecvd);
                if (0 > recvd) {
                    printf("content recv failed, break\n");
                    break;
                }
#ifdef DEBUG
                printf("Received: %s\n", content+contentRecvd);
#endif
                contentRecvd += recvd;

            }
        }

        if (nullptr != pRecvCallback_) {
            pRecvCallback_(identity, content, identityRecvd+contentRecvd);
        }
    }
#ifdef DEBUG
    printf("exit RecvDaemon\n");
#endif
}


int Client::send(const void *buf, unsigned int len, int flag)
{
    return zmq_send(zmqSocket_, buf, len, flag);
}

int Client::recv(void *buf, unsigned int len)
{
    return zmq_recv(zmqSocket_, buf, len, 0);
}



