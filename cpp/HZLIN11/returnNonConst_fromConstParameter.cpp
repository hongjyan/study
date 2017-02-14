#include <string>
#include <iostream>

struct Bar {
    std::string mdata;
};

class Foo {
public:
    Foo(Bar& bar): mbar(bar) {}

    Bar& getBar() {
        return mbar;
    }
 

    static Bar& getBar(const Foo& foo) {
        return foo.mbar;
    }

private:
    Bar& mbar;
};


int main() {
    Bar bar;
    bar.mdata = "xiaoming";
    const Foo foo(bar);
    Bar& rbar = Foo::getBar(foo);
    rbar.mdata = "xiaoli";
    std::cout << "bar.mdata is " << bar.mdata << ", rbar.mdata is " << rbar.mdata << std::endl;
    return 0;
}
