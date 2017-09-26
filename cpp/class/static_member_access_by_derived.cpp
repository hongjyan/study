#include <iostream>
using namespace std;

class Base {
public:
    static const char *pub_str;
    static const int val = 10;
};
const char* Base::pub_str = "C++";
const int Base::val;

struct Derived : public Base {
     void output() {
	cout << pub_str << endl;
	cout << val << endl;
    }
};

int main() {
    Derived d;
    d.output();
    cout << Derived::pub_str << endl;
    cout << Derived::val << endl;
}       
