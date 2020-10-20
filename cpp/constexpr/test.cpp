#include <iostream>
#include <cstdio>
using namespace std;

class Debug {
public:
  constexpr Debug(bool b = true): hw(b), io(b), other(b) {}
  Debug (bool h, bool i, bool b = true): hw(h), io(i), other(b) {}

  bool gethw() const { return hw; }
  bool getio() const { return io; }
  bool getother() const { return other; }
  void sethw(bool b) { hw = b; }

private:
  bool hw;
  bool io;
  bool other;
};



int main() {
  constexpr int *p = nullptr;
  int i = 0;
  //p = &i; #p's value is fix during compile-time.

  constexpr Debug d(false); //use constexpr construtor to initialize
  //constexpr Debug d = new Debug(false); //error: call to non-constexpr function 'void* operator new(std::size_t)'
  cout << "d.gethw() is " << d.gethw() << endl;
  //d.sethw(true); //Q: is it possbile to change constexpr value? A: NO, since constexpr object imply const in c++11.

  //Debug instance can be constexpr or not, depends on way of initialize
  Debug d2(false, false, false);
  d2.sethw(true); //constexpr just need value evaluated during compiled. 

  return 0;
}
