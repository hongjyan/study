#include <iostream>
using namespace std;


class Foo {
public:
  Foo() { cout << "ctr" << endl; }
private:
  ~Foo() { cout << "de-ctr" << endl; }
};

class Bar {
public:
  Bar() { cout << "bar-ctr" << endl; }

  friend void destructBar(const Bar * const ptr);
private:
  ~Bar() { cout << "bar-dctr" << endl; }
};

void destructBar(const Bar * const ptr) {
  delete ptr; //it is ok for friend function to call private member
}

class Tra {
public:
  Tra() { cout << "tra-ctr" << endl; }
  ~Tra() = delete;
};


class Tar {
public:
  static void tar();
private:
  Tar() = delete;
};



int main() {
  //Foo f; //error. since f's dctor will be call when it is dieing, but Foo's dctor is private
  Foo *p = new Foo(); //memory leak
  //delete p; //error. since dctor will be called when delete the object. but if we don't delete p, that will lead memory leak. Then how to deal with this?

  Bar *p2 = new Bar();
  //delete p2; //error: 'Bar::~Bar()' is private
  destructBar(p2);

  //Tra t;  //error: use of deleted function 'Tra::~Tra()'
  Tra *p3 = new Tra(); //memory leak
  //delete p3;  //error: use of deleted function 'Tra::~Tra()'
  

  //Tar *p4 = new Tar(); //error: 'Tar::Tar()' is private. error: use of deleted function 'Tar::Tar()'
  return 0;
}
