#include <iostream>
using namespace std;

class A {
public:
    A(int);
    void output();
private:
    const int SIZE;   //const variable for object.
//    const int TEST = 12;  //ISO C++ forbids initialization of member, if want to set a const value for class\
			    can set static const int TEST = 12;
    enum { SIZE1 = 100, SIZE2 = 200 };  //use enum type to realize const variable for class.
    int m_a[SIZE1];
};

A::A(int size): SIZE(size) {};

void A::output() {
    cout<<"SIZE is "<<SIZE<<endl;
//    cout<<"TEST is "<<TEST<<endl;
    cout<<"size of array is "<<sizeof(m_a)<<endl;
}



int main() {
    A a(22);
    a.output();
}
    
