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

    buf.mtype = 2;


    if ((key = ftok("sunriver", 'B')) == -1 ) {
        perror("ftok");
        exit(-1);
    }

    if ( (msqid = msgget(key, 0644 | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(-1);
    }


    while (fgets(buf.mtext, sizeof(buf.mtext), stdin) != NULL) {
         int len = strlen(buf.mtext);
         if (msgsnd(msqid, &buf, len, 0) == -1) {
             perror("msgsnd");
             exit(-1);
         }
         
    }

    if (msgctl(msqid, IPC_RMID, NULL) == -1) {
        perror("msgcntl");
        exit(-1);
    }

    return 0;
}
