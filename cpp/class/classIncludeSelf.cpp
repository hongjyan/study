#include <iostream>
using namespace std;

class foo;

// error: â€˜fâ€™ has incomplete type
// foo func(foo f){
//     return f;
// }

foo* func2(foo* pf){
    return pf;
}

struct Bar
{
    Bar() { std::cout << "ctor" << std::endl; }

    Bar getDefaultBar()
    {
        return Bar();
    }

    static Bar staticllyGetDefaultBar()
    {
        return Bar();
    }

private:
    // Bar self_; // will cause Bar infinitely large
    Bar* pself_;  // ok
};


int main() {
    Bar bar;
    bar.getDefaultBar();
    Bar::staticllyGetDefaultBar();
    return 0;
}  
