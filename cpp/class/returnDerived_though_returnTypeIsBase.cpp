#include <iostream>
#include <vector>
using namespace std;

class Base {
public:
  virtual void say() const {
    cout << "I am base" << endl;
  }
  
  virtual ~Base() {}
};

class Derived : public Base {
public:
  virtual void say() const {
    cout << "I am Derived" << endl;
  }
};


Base foo() {
  Derived d;
  return d;
}


Base* foo2() {
  Derived *p = new Derived();
  return p; 
}


//this method should n't be here since undefined behavior to return reference to temporary object
Base& foo3() {
  Derived d;
  return d; //undefined behavior
}


int main() {
  Base b = foo();
  b.say();

  Base *p = foo2();
  p->say();
  delete p;

  Base &b3 = foo3();
  b3.say();
  return 0;
}
