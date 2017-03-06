#include "unp.h"
#include "sum.h"

void str_echo(int sockfd)
{
    struct args params;
    struct result result;
    int n = sizeof(params);
    int read;

    for ( ; ;) {
	if ( (read = readn(sockfd, &params, n)) == 0) {
	    Printf("str_echo: peer terminate prematurely\n");
	    return;
	}
	else if (read < 0) {
	    Printf("str_echo: read error\n");
	    return;
	}
	else if (read != n) {
	    printf("str_echo: read not enough data\n");
	    return;
	}

	result.sum = params.arg1 + params.arg2;
	printf("result is %ld\n", result.sum);
	Writen(sockfd, &result, sizeof(result));
    }
}
