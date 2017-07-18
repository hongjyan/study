#include <cstdio>
#include <string>

class Foo {
public:
    Foo() { printf("Constructing Foo\n"); }

private:
    class Impl;
    Impl* _impl;
};

class Foo::Impl {
public:
    Impl() { printf("Constructing Foo::Impl\n"); }

    static std::string say() {
        return std::string("Foo::Impl");
    }
    
    friend void someFriend() {
        //printf("I am some friend of %s\n", Foo::Impl::say().c_str()); //wrong
        printf("I am some friend of %s\n", Impl::say().c_str()); //ok
        printf("I am some friend of %s\n", say().c_str()); //ok
        
    }
};

void someFriend();


int main() {
    someFriend();
}
