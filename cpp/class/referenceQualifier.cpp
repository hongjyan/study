#include <iostream>
#include <cstdarg>
#include <vector>
#include <algorithm>

using namespace std;

class Foo {
public:
  Foo() = default;
  Foo(vector<int> ivec): data(ivec) {}

  void fillData(int num,  ...) {
    va_list valist;
    va_start(valist, num);
    for(int i=0; i<num; i++) {
      data.push_back(va_arg(valist, int));
    }
    va_end(valist);
  }

  Foo sorted() const & {
    Foo ret(*this);
    sort(ret.data.begin(), ret.data.end());
    return ret;
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
  Foo f;
  f.fillData(3, 3, 2, 1);
  f.print();

  f.sorted().print();

  vector<int> ivec{33, 22, 11};
  Foo(ivec).sorted().print();  //call sorted on a rvalue.

  return 0;
}
