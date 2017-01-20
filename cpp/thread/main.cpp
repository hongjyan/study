#include "stub.hpp"
#include "lib.hpp"
#include <iostream>

int main() {
    Stub stub;
    stub.setName("xiaoming");
    Lib lib;

    lib.start(1, stub.handler);
    
    std::cout << stub.getName() << std::endl;
}
    
