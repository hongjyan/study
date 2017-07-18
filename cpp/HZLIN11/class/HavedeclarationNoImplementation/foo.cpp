#include "foo.hpp"
void foo::print() {
    printf("foo\n");
}

int main() {
    foo fo;
    fo.print();
}
