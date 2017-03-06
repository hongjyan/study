#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct my_msgBuf {
    long mtype;
    char mtext[200];
};


int main() {
    struct my_msgBuf buf;
    key_t key;
    int msqid;


    if ((key = ftok("sunriver", 'B')) == -1 ) {
        perror("ftok");
        exit(-1);
    }

    if ( (msqid = msgget(key, 0644)) == -1) {
        perror("msgget");
        exit(-1);
    }


    do {
        if (msgrcv(msqid, &buf, sizeof(buf.mtext), 0, 0) == -1) {
             perror("msgrcv");
             exit(-1);
         } 
         printf("%s", buf.mtext);
    }
    while (1);  

    return 0;
}
