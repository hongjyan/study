#include <stdio.h>

//Sigfunc is a function
typedef void  Sigfunc(int);
//Sigfunc_t is a function pointer 
typedef void (*Sigfunc_t)(int);

void sig_chld(int signo) {
    printf("sig_chld\n");
}

//Do not use typedef, looks mess.
//if a function return a "function pointer", * must be adjacent to function name closely.
void (*signal(int signo, void (*sig_handler) (int))) (int) {
    sig_handler(0);
    printf("sighandler address is %p\n", sig_handler);
    return sig_handler;
}

//use typedef  function
Sigfunc* signal_func(int signo, Sigfunc* func) {
    func(0);
    printf("func address is %p\n", func);
    return func;
}

//use typedef function pointer
Sigfunc_t signal_func_t(int signo, Sigfunc_t func_t) {
    func_t(0);
    printf("func_t address is %p\n", func_t);
    return func_t;
}
    

//return a "function pointer" which has not any parameter & return nothing.
void foo() {
    printf("foo\n");
}

void (*returnfoo(int x)) () {
    printf("returnfoo\n");
    return foo;
}

//return a "function pointer" which has not any parameter & return a void* pointer
void* bar() {
    printf("bar\n");
    return NULL;
}

void* (*returnbar(int x)) () {
    printf("returnbar\n");
    return bar;
}


int main() {
    Sigfunc *pfunc; //must add * before pfunc
    Sigfunc_t func_t;

//c standard says that a function name in this context is converted to the address of function, so,
//it is ok to pass sig_chld or &sig_chld as the second parameter of signal, signal_xx
    pfunc = signal(0, sig_chld);
    func_t  = signal(0, sig_chld);
    printf("pfunc  is %p\n", pfunc);
    printf("func_t is %p\n", func_t);

    pfunc = signal_func(0, sig_chld);
    func_t  = signal_func(0, sig_chld);
    printf("pfunc is %p\n", pfunc);
    printf("func_t is %p\n", func_t);
    
    pfunc = signal_func_t(0, sig_chld);
    func_t = signal_func_t(0, sig_chld);
    printf("pfunc is %p\n", pfunc);
    printf("func_t is %p\n", func_t);

    returnfoo(0)();
    returnbar(0)();
}
