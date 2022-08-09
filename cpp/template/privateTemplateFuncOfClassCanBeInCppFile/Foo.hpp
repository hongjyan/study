#include <iostream>

class Foo {
public:
    //template<typename T> void interface1(T t);
    template<typename T> void interface1(T t) { std::cout << __func__ << std::endl; }
    void interface2();
private:
    template<typename T> void privateFunc(T t);
};
