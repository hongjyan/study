#include <memory>
#include <system_error>
#include <cstdio>

struct Foo {
    Foo() { printf("Constructing Foo\n"); }
    void bar() { printf("Foo::bar()\n"); }
    ~Foo() { printf("Destructing Foo\n"); }
};

std::unique_ptr<Foo> createFoo() {
    std::unique_ptr<Foo> up(new Foo);
    return up;
}

int main() {
    std::unique_ptr<Foo> upFoo = createFoo();
    upFoo->bar();
}

   



