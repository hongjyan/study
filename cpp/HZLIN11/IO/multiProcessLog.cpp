#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>


#define PROCESS_NUM 500
#define THREAD_NUM 100
#define PRINT_TIMES 50

struct logParams {
    int fd;
    char* text;
};

void* log(void* param) {
    int times = PRINT_TIMES;
    struct logParams *lp = (struct logParams *) param;
    pid_t pid = getpid();
    pthread_t tid = pthread_self();

    do {
	fprintf(fdopen(lp->fd, "w"), "%d: my pid is %d, my tid is %d, text is %s\n", 51-times, pid, tid, lp->text);
    }
    while (times--);

    return 0;
}


int process(char* filename, char* text) {
    struct logParams lp;
    int fd;
    int	thread_num = THREAD_NUM;
    pthread_t pids[thread_num];
   

    if (-1 == (fd = open(filename, O_CREAT | O_RDWR | O_APPEND, S_IRWXU))) { 
	perror("open");
	return -1;
    }

    lp.fd = fd;
    lp.text = text;
    
    do {
	if (0 != pthread_create(&pids[thread_num], 0, log, &lp)) {
	    perror("pthread_create");
	    return -1;
	}
    }
    while (thread_num--);

    for (int i=0; i<THREAD_NUM; i++) {
	pthread_join(pids[i], 0);
    }
    
    return 1;
}


    


int main() {
    char filename[200];
    char* text = NULL;
    size_t n;
    pid_t pid;
    
    printf("input filename\n");
    fgets(filename, 200, stdin);
    printf("input text\n");
    if (-1 == getline(&text, &n, stdin)) {
	perror("geline");
	_exit(-1);
    } 

    for (int i=0; i<PROCESS_NUM; i++) {
	pid = fork();
	if (0 == pid) {
	    process(filename,  text);
	    return 0;
	}
	else if (0 < pid) {
	    usleep(100000);
	}
	else {
	   perror("fork");
	   _exit(-1);
	}
    }
    return 0;
}
	    

