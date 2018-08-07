#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void usage(void) {
    printf("usage:\n   launch.o sever/msrp_server.out|client/msrp_client.out $ip $startPort, $endPort, $file_to_sent\n");
}

int main(int argc, char* argv[]) {
    if (argc < 6) {
        usage();
    }
    
    char *interval = "100";
    int startport = atoi(argv[3]);
    int endport = atoi(argv[4]);
    char char_startport[30];
    sprintf(char_startport, "%d", startport);
    char *newargv[6] = { argv[1], argv[2], char_startport, interval, argv[5], NULL};
    char *envp[] = { NULL };
    
    
    while (startport < endport) {
        pid_t pid = fork();
        if (pid >0) {
            startport += 100;
            sprintf(newargv[2], "%d", startport);
        }
        else if (pid == 0) {
            for(int i=0; i<6; i++) {
                printf("%s ", newargv[i]);
            }
            printf("\n");
            execve(argv[1], newargv, envp);
            break;
        }
        else {
            printf("error happened during fork\n");
            break;
        }
    }
} 
