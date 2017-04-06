#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>

using namespace std;

//2017.4.6
class Foo {
public:
    Foo(const std::string& name): name_(name) {}
    //void setName(const std::string& name) { name_ = name; } //wrong, can not change const reference content
private:
    const std::string& name_;
};

int main() {
    vector<int> ivec;
    std::string bar("foo");
//2017.3.20, will overflow to assign big value
    char c = 12222;  //overflow
    int i = 12222;
    c = i; // not overflow

//2017.3.22 
    cout << sizeof(long) << endl;
//2017.3.28
    unsigned int j = atoi("game");
    cout << j << endl;
    j = atoi("-1");
    cout << j << endl;
//2017.4.6
//    Foo foo("game");
//    foo =  Foo("fun");

    return 0;
}
