#include <stdio.h>

class Bar {
public:
    void constF() const {
        printf("I am in Bar::constF()\n");
    }
};

class Hey {
public:
    void F() {
        printf("I am in Hey::F()\n");
    }
};

class Foo {
public:
    void F() {
        printf("I am in Foo::F()\n");
    }        

    //C++ standard allows overloading of functions that differ only by the constness no matter const qualify function parameter or return type
    void F() const {
        printf("I am in Foo::F() const\n");
    }

    void fun(const Foo &object) {
        printf("I am in fun(const Foo&)\n");
    }
    void fun(Foo &object) {
        printf("I am in fun(Foo &)\n");
    }
};

int main(int agrc, char* argv[]) {
    Bar bar; 
    bar.constF();  //non-const object can call const function.

    const Hey hey = Hey(); 
    //hey.F();  //Wrong, const object can not call non-const function. 

    Foo nonConst_foo; 
    const Foo const_foo = Foo();
    nonConst_foo.F(); //non-const object select non-const function firstly
    const_foo.F();    //const object select const function firstly
    nonConst_foo.fun(nonConst_foo);
    nonConst_foo.fun(const_foo);  //non-const object can call const-function also.
}
