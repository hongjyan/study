//answer is no.
//yes, in scope of MyClass, private is accessiable. OtherClassut it is not accessable in scope of other class
// since  member function of other class doesn't aware private member of MyClass
#include <memory>
#include <iostream>
using namespace std;


class MyClass {
    // MyClass() { cout << __func__ << endl; };  //NOK, don't know why
    MyClass() = default; //OK
public:
    static MyClass* create() {
        return new MyClass(); 
    }
    //wrong, since MyClass() is inaccessiable for make_shared. like OtherClass::createMyClass() is illegal.
    // static shared_ptr<MyClass> createSp() { return make_shared<MyClass>(); }

    void print() { cout << "hi" << endl; }
};

class OtherClass {
public:
    // static MyClass* createMyClass() { return new MyClass(); }
};

class DerivedMyClass : public MyClass { };

shared_ptr<MyClass> make_a() {
    return make_shared<DerivedMyClass>();
}

int main() {
    auto p = MyClass::create();
    p->print();
    delete p;

    auto sp = make_a();
    sp->print();

}