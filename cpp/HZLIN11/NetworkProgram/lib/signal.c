#include "unp.h"

Sigfunc* 
Signal(int signo, Sigfunc* sighandler) {
    struct sigaction    oldact, act;
    act.sa_handler = sighandler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
/*
    if (signo == SIGALRM) {
#ifdef SA_INTERRUPT
	act.sa_flags |= SA_INTERRUPT;
#endif
    } else {
#ifdef SA_RESTART
	act.sa_flags |= SA_RESTART;
#endif
    }
*/
    
    if (sigaction(signo, &act, &oldact) < 0) 
	return SIG_ERR;
    return oldact.sa_handler;
}

/*
Sigfunc* 
Signal(int signo, Sigfunc* handler) {
    Sigfunc *oldhandler;
    if ( (oldhandler = signal(signo, handler)) == SIG_ERR) { 
	Printf("signal fail\n");
	return SIG_ERR;
    }
    return oldhandler;
}
*/
    
    
