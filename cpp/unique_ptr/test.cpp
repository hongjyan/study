#include <iostream>
#include <memory>
using namespace std;

class Foo {
public:
  Foo() { cout << "ctr" << endl; }
  ~Foo() { cout << "dctr" << endl; }
};

int main() {
  unique_ptr<Foo[]> up(new Foo[10]());
  //up.release();  //wrong, will lead memory leak
  up.reset();  //can use reset to call delete on Foo[10]
  //delete [] p;  //coredump. delete Foo[10] twice
  cout << "---------------------------" << endl;
  
  unique_ptr<Foo[]> up2(new Foo[10]()); //unique_ptr will delete the memory it points when out of its life.
  cout << "---------------------------" << endl;

  return 0;
}
