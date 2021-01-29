#include <iostream>
using namespace std;

class Foo {
public:
  Foo() = default;
  Foo(const Foo&) = delete;
};

class Base {
public:
  Base(int i): _i(i) { 
    cout << "Base(int, int&)" << endl; 
  }

  Base(const Base& src): _i(src._i) { 
    cout << "Base(const Base &)" << endl; 
  }

  virtual void print() {
    cout << "_i is " << _i << endl;
  }

private:
  int _i;
  Foo f;   //including Foo to prevent synthesized copy-construter for Derived to test if Derived class can inherit Base class's copy-constructer by using Base::Base
};


class Derived : public Base {
public:
  using Base::Base;

  void print() {
    Base::print();
    cout << "_k is " << _k << endl;
  }

private:
  double _k;
  Foo f;
};


int main() {
  Derived d(1);  //ok in C++11 together with using Base::Base;
  d.print();

  //Derived d2(d); //error: use of deleted function 'Derived::Derived(const Derived&)'. Which indicate Derived class can not inherit Base class's copy-constructer even with "using Base::Base"
  cout << endl << endl << endl;

  return 0;
}
