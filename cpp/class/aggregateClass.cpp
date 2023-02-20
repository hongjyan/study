#include <string>
#include <functional>
#include <iostream>

struct Foo
{
    int i;
    char c;
    std::string s;
    std::function<void(int)> callback;
};

struct FooWithDefaultValue
{
    int i{1};
    char c{'c'};
    std::string s{"hello"};
};

struct Bar {
    Bar(int i, char c, std::string s, std::function<void()> callback) {}
private:
    int i;
    char c;
    std::string s;
    std::function<void()> callback;
};

int main() {
    Foo foo{1, 'c', "hello"};
    std::cout << foo.s << std::endl;

    Foo foo2{1};
    std::cout << foo2.c << std::endl;
    std::cout << foo2.s << std::endl;

    Foo foo3;
    std::cout << foo3.i << std::endl; //遵循默认初始化原则。 built-in类型在局部作用域不会初始化，所以值未定义

    Foo foo4{1, 'c', "hi", [](int){}};
    foo4.callback(3);
    // Bar bar(1, 'c'); //no matching function for call to â€˜Bar::Bar(int, char)

    FooWithDefaultValue fooWithDefaultValue;
    //In c++11, no matching function for call to â€˜FooWithDefaultValue::FooWithDefaultValue(<brace-enclosed initializer list>)
    //Ok in c++14
    FooWithDefaultValue fooWithDefaultValue1 = {2, 'd', "fun"};
    std::cout << "fooWithDefaultValue1.s is " << fooWithDefaultValue1.s << std::endl;
    FooWithDefaultValue fooWithDefaultValue2 = {2, 'd'};
    std::cout << "fooWithDefaultValue2.s is " << fooWithDefaultValue2.s << std::endl;
}