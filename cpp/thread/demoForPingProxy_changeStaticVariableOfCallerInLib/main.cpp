#include "stub.hpp"
#include "lib.hpp"
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>


void printName(void* stub) {
     Stub* pstub = (Stub*)stub;
     std::cout << pstub->getName() << std::endl;
}

int main() {
    Stub stub;
    stub.setName("xiaoming");
    Lib lib;
    //pthread_cleanup_push(printName, &stub);

    lib.start(1, stub.handler);

    sleep(1);
    printName(&stub);
    
    //pthread_exit(0);
   // pthread_cleanup_pop(0);
   return 0;
}
    
