#include <iostream>
using namespace std;

class foo {
public:
    void f() {
	cout << "I am f() in foo" << endl;
    }
};


class bar : public foo {
public:
    void ff() {
	foo::f();
    }
};

class tar {
public:
    void ff() {
//	foo::f();
    }
};

int main() {
    bar b;
    b.ff();

    tar t;
    t.ff();
}

