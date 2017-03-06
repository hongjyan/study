#include <iostream>
using namespace std;

class A {
public:
    void Func(void) { cout<<"Func of class A"<<endl; }
};

void Test(void) {
    A *p;
    {	A a;
	p = &a;
    }
    p->Func();  //problem should happen here. But seems not!
}

int main() {
    Test();
}
