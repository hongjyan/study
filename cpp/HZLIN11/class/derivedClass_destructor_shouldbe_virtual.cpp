#include <iostream>
using namespace std;

class Base {
public:
    Base() { cout<<"Base construct"<<endl; }
     ~Base() { cout<<"~Base"<<endl; }
};

class Derived : public Base {
public:
    Derived() { cout<<"Derived construct"<<endl;}
     ~Derived() { cout<<"~Derived"<<endl; }
};


class Base2 {
public:
    Base2() { cout<<"Base2 construct"<<endl; }
     ~Base2() { cout<<"~Base2"<<endl; }
};

class Derived2 : public Base2 {
public:
    Derived2() { cout<<"Derived2 construt"<<endl; }
     ~Derived2() { cout<<"~Derived2"<<endl; }
};

int main() {
    Base *pb = new Derived;
    delete pb; //this line will call the destructor of pb:pb->~Base(), since ~Base() is not virtual, so\
		dynamic binding won't take use. so only destructor of base class was called. This is wrong,\		    because derived class construct was called but derived class destruct was omited. This is \
		the reason why we set virtual for destructor of base class. 
    Derived2 d2; //destructor of derived class will call its base class's destructor.
}
/* output is 
 * Base construct
 * Derived construct
 * ~Base
 * Base2 construct
 * Derived2 construt
 * ~Derived2
 * ~Base2
 */
