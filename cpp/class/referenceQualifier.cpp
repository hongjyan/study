#include <iostream>
#include <cstdarg>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

class Foo {
public:
  Foo() = default;
  Foo(vector<int> ivec): data(ivec) {}

  Foo(const Foo &f): data(f.data) {}

  Foo(Foo &&f) = default;
  /*Foo(Foo &&f) noexcept { 
    data = std::move(f.data);
    cout << "Foo(Foo &&f)" << endl;
  }*/

  Foo& operator=(const Foo &rhs) & {
    //cout << "operator=" << endl;
    data = rhs.data;
    return *this;
  }

  void fillData(int num,  ...) {
    va_list valist;
    va_start(valist, num);
    for(int i=0; i<num; i++) {
      data.push_back(va_arg(valist, int));
    }
    va_end(valist);
  }

  Foo sorted() & {
    cout << "sorted() &" << endl;
    Foo ret(*this);
    sort(ret.data.begin(), ret.data.end());
    return ret;
  }

  Foo sorted() const &{
    cout << "sorted() const &" << endl;
    Foo ret(*this);
    sort(ret.data.begin(), ret.data.end());
    return ret;
  }

  Foo& sorted() && {
    cout << "sorted() &&" << endl;
    sort(data.begin(), data.end());
    return *this;
  }

  void print() const {
    for(auto &e: data) {
      cout << e << " ";
    }
    cout << endl;
  }

  const Foo getCopy() {
    return Foo(*this);
  }

private:
  vector<int> data;
};


int main() {
  cout << "not ok to assign value to rval since operator= has reference-qualifier:&" << endl;
  string() = "abc"; 
  //Foo() = f; //not ok to assign value to rval since operator= has reference-qualifier:&
  cout << endl << endl << endl;


  cout << "it is OK to call 'sorted() &' on lval" << endl;
  Foo f;
  f.fillData(3, 2, 3, 1);
  f.print();
  f.sorted().print();
  cout << endl << endl << endl;

   
  cout << "first match: 'sorted() &&'." << endl;
  cout << "second match:'sorted() const &' since it is ok for const int &i = 20(it is ok to bond a const reference to rvalue." << endl;
  vector<int> ivec{33, 22, 11};
  Foo(ivec).sorted().print();
  cout << endl << endl << endl;
  const int &i = 2;
  cout << endl << endl << endl;

 

  cout << "call 'sorted() &' on const rvalue" << endl;
  f.getCopy().sorted().print();
  cout << endl << endl << endl;
 


  cout << "call 'sorted() &' on rvalue-reference since reference always has a name" << endl;
  Foo &&rf = std::move(f);
  rf.sorted().print();
  cout <<  "but 'sorted() && on rvalue" << endl;
  std::move(f).sorted().print(); //move cast f to rvalue.
  f.print(); //std::move just convert lval to rval reference. 
  Foo f2(std::move(f)); //move-contructor called since std::move(f) is &&, so f will be empty.
  f2.print();
  cout << "f.print is ";
  f.print();
  cout << endl << endl << endl;

  return 0;
}
