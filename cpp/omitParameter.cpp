#include <iostream>

void f(int = 1) {
    std::cout << '1' << std::endl;
}

int main() {
    f(); //compile error.
    f(4);
}