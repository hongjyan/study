#include <iostream>
#include <memory>

struct Foo
{
    Foo() { std::cout << "ctor" << std::endl; }
    ~Foo() { std::cout << "dector" << std::endl; }
};

int main()
{
    auto p = new Foo;
    {
        auto sp1 = std::make_shared<Foo>(p);
    }
    {
        auto sp1 = std::make_shared<Foo>(p);
    }
    delete p;
    return 0;
}