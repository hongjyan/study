#include "unp.h"

void sig_pipe(int signo)
{
    printf("catch SIGPIPE\n");
    return;
}
