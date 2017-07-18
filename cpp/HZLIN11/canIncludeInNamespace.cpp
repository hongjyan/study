#include <stdio.h>
#include <vector>
#pragma GCC visibility push(hidden)
#include <boost/asio/ip/address.hpp>
#pragma GCC visibility pop

namespace TEST {

//#include <vector>  //Can not include standard header file in namespace
class Foo {
public:
    Foo() { }
    
//#include <vector>     //Can not include standard header in namespace::class
    void f() {
//        std::vector<int> vi;
    }

    boost::asio::ip::address getIPAddress() { return boost::asio::ip::address(); }
};

}

class Bar {
public:
    Bar() {}

//#include <vector>    //can not include standard header in class
    void f() {
//        #include <vector>   //can not include standard header in class::function
//        std::vector<int> vi;
        printf("I am Bar::f()\n");
    }
};

void f() {
    //#include <vector>    //can not include standard header in function
   // std::vector<int> vi;
    printf("I am f()\n");
}

int main() {
    TEST::Foo foo;  foo.f(); foo.getIPAddress();
    Bar bar; bar.f();
    f();
}
