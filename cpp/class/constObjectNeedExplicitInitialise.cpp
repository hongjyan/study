#include <stdio.h>

class Foo {
};

class Bar {
public:
    Bar() {}
};

int main() {
    Foo foo_object;
    const Foo const_foo_object; //wrong, need explicte initialise
    //const Foo const_foo_object = Foo();

    Bar bar_object;
    const Bar const_bar_object;  //ok, since Bar has its own user-define default constrcutor
}
