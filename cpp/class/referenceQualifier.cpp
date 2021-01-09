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

  Foo& operator=(const Foo &rhs) & {
    cout << "operator=" << endl;
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

  Foo sorted() && {
    cout << "sorted() &&" << endl;
    sort(data.begin(), data.end());
    return *this;
  }

  void print() {
    for(auto &e: data) {
      cout << e << " ";
    }
    cout << endl;
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
  Foo(ivec).sorted().print();  //why call "sorted() &" OK on a rvalue?
  cout << endl << endl << endl;
  const int &i = 2;
  cout << endl << endl << endl;

  return 0;
}
