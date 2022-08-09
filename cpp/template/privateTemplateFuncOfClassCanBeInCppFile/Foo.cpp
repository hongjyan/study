#include "Foo.hpp"

//template <typename T> void Foo::interface1(T t) {
//    std:: cout << __func__ << std::endl;
//}

void Foo::interface2() {
    privateFunc(1);
}

template <typename T> void Foo::privateFunc(T t) {
    std:: cout << __func__ << std::endl;
}
