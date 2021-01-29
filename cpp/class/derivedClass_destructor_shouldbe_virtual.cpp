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
    virtual ~Base2() { cout<<"~Base2"<<endl; }
};

class Derived2 : public Base2 {
public:
    Derived2() { cout<<"Derived2 construt"<<endl; }
     ~Derived2() { cout<<"~Derived2"<<endl; }
};

class DD: public Derived2 {
public:
    DD() { cout << "dd ctr" << endl; }
    ~DD() { cout << "dd ~ctr" << endl; }
};

int main() {
    Base *pb = new Derived;
    delete pb; //this line will call the destructor of pb:pb->~Base(), since ~Base() is not virtual, so dynamic binding won't take use. so only destructor of base class was called. 
    Derived d; //destructor of derived class will call its base class's destructor where dynamic binding isn't be used.
    cout << endl << endl << endl;
    
    cout << endl << endl << endl;
    Base2 *pb2 = new Derived2();
    delete pb2;
    Base2 *pdd = new DD();  
    delete pdd;   //dynamic binding still take effect even Derived2(direct base class) hasn't declare virtual for dctor but Base2(root base class)'s dctor is virtual.


    cout << endl << endl << endl;
}
