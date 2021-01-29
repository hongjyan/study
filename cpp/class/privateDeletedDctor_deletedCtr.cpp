#include <iostream>
#include <memory>
using namespace std;

//private dctor class
class Foo {
public:
  Foo() { cout << "ctr" << endl; }
private:
  ~Foo() { cout << "de-ctr" << endl; }
};

//private dctor class with public member function which call that private dctor
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

//delete dctor class
class Tra {
public:
  Tra() { cout << "tra-ctr" << endl; }
  ~Tra() = delete;
};

//private & deleted constructor
class Tar {
public:
  static void tar();
  Tar() = delete;
};



int main() {
  //Foo f; //error. since f's dctor will be call when it is dieing, but Foo's dctor is private
  Foo *p = new Foo(); //memory leak
  //delete p; //error. since dctor will be called when delete the object. but if we don't delete p, that will lead memory leak. Then how to deal with this?

  Bar *p2 = new Bar();
  //delete p2; //error: 'Bar::~Bar()' is private
  destructBar(p2);
  //Bar b;  //error: 'Bar::~Bar()' is private

  //Tra t;  //error: use of deleted function 'Tra::~Tra()'
  Tra *p3 = new Tra(); //memory leak
  //delete p3;  //error: use of deleted function 'Tra::~Tra()'
  

  //Tar *p4 = new Tar(); //error: use of deleted function 'Tar::Tar()'
  //neigther place-new deleted-contructor class!
  /*
  allocator<Tar> alloc;
  void *rawPlace = alloc.allocate(sizeof(Tar));
  Tar *p5 = new(rawPlace) Tar;
  alloc.destroy(p5);
  alloc.deallocate(p5, sizeof(Tar));
  */
  return 0;
}
