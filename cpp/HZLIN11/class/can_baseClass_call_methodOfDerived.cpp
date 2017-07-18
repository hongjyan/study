#include <iostream>
using namespace std;

class Base {
public:
    Base() { }

    void f() {
        f_toBeCall();
//      f_toBeCall_nonVitrual();   //obviously, can't call a non-virtual function which is defined in derived class since base class don't know it exist.
    }

    virtual void f_toBeCall()
    {
        cout << "f_toBeCall in base class" << endl;
    }


    //added in 2015.10.21
    void pointer_call(void* p) {
	static_cast<Base *> (p)->f_toBeCall();  //The reason why static_cast here is that p is void*, so can not call any funcation. Notice that f_toBeCall
                                                // will be the derived version even p was static_cast to Base if p is derivedClass-pointer.
    }

};

class Derived : public Base {
public:
    void f_toBeCall() {
        cout << "f_toBeCall in derived class" << endl;
    }

    void f_toBeCall_nonVirtual() {
        cout << "f_toBeCall_nonVirtual in derived class" << endl;
    }
};

class pureV_base {
public:
    void f() {                      //f() call function defined in derived class.
        f_toBeCall();
    }

    virtual void f_toBeCall() = 0;  //define pure virtual function here to let derived class rewrite it. 
};

class pureV_Derived : public pureV_base {
public:
    void f_toBeCall() {
        cout << "I am in derived class" << endl;
    }
};

int main() {
    Base B;
    B.f();

    Derived D;
    D.f();   // the f() inherited from base call f_toBeCall() & f_toBeCall2() from derived class.

    pureV_Derived DV;
    DV.f();   
    
    //added at 2015.10.21
    B.pointer_call(&D);

};
