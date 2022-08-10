#include <iostream>

int foo() { 
    return 10;
}
int main() {
    int i = 0;
    int& j = i;
    int& k = j;
    i = 10;
    std::cout << i << std::endl;
    std::cout << j << std::endl;
    std::cout << k << std::endl;
    k = 9;
    std::cout << i << std::endl;
    std::cout << j << std::endl;
    std::cout << k << std::endl;
    int h = 12;
    j = foo();
    return 0;
}