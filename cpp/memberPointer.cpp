#include <iostream>
#include <string>
using namespace std;

class Foo {
public:
  bool gt(int a, int b) const { return a>b; }
  bool gt(string a, string b) const { return a > b; }
  char test(int i) { return 'c'; }

  static const string Foo::* get_ps() { //get pinter to member s
    return &Foo::s;
  }

  int i;
  int k;


private:
  const string s;
};

int main() {
  //variable member pinter
  int Foo::*pmv;                //declaration. undertermized to pint to i or k
  pmv = &Foo::i;               //initialization. pinter to i. 
                               //Foo here is redundant a bit here. But better than "p = &i;"
  auto pmv1 = &Foo::i;         //declaration and initialization once
  int Foo::*pmv2 = &Foo::i;    //declaration and initialization once

  Foo obj;
  Foo *p = &obj;
  cout << obj.i << endl; 
  cout << obj.*pmv << p->*pmv << endl; //use. Must specify a object(f) to pinter(p)
  cout << obj.*pmv1 << p->*pmv1 << endl;
  cout << obj.*pmv2 << p->*pmv2 << endl;

  auto pmv3 = Foo::get_ps();  //nomally, varaible member are private, disallow to access directly.
                             //usually, use function to get it
  const string Foo::*pmv4 = Foo::get_ps();
  cout << obj.*pmv3 << p->*pmv3 << endl;
  cout << obj.*pmv4 << p->*pmv4 << endl;


  //function member pinter
  char (Foo::*pmf) (int);                           //declaration
  pmf = &Foo::test;                                 //initialization
  auto pmf2 = &Foo::test;                           //declaration and initialization
  //auto pmf3 = &Foo::gt;         //wrong! must specify function type if function overloadded
  bool (Foo::*pmf3)(string, string) const = &Foo::gt;

  cout << (obj.*pmf)(2) << (p->*pmf)(2) << endl;    //use
  cout << (obj.*pmf2)(2) << (p->*pmf2)(2) << endl;  //use
  cout << (obj.*pmf3)("abc", "bcd") << (p->*pmf3)("abc", "bcd") << endl;  //use

  using GTS = bool (Foo::*) (string, string) const; //use using to simplify member function declaration
  GTS gts = &Foo::gt;
  cout << (obj.*gts)("abc", "bcd") << (p->*gts)("abc", "bcd") << endl;


  return 0;
}

