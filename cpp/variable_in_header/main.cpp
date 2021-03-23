#include "main.hpp"
#include <iostream>
using namespace std;

int main() {
  cout << "c_foo:" << c_foo << endl;
  cout << "c_foo**:" << afterward_square(c_foo) << endl;
  cout << "c_foo:" << c_foo << endl;
  cout << "c_foo++:" << afterward_plus(c_foo) << endl;
  cout << "c_foo:" << c_foo << endl;
  cout << endl << endl << endl;

  cout << "bar:" << bar  << ", &bar:" << &bar << endl; //6, main's bar
  cout << "kar:" << kar  << ", &kar:" << &kar << endl; //6, main's kar
  cout << "tar:" << tar  << ", &tar:" << &tar << endl; //6, global tar
  
  cout << endl << endl << endl;
  forward_square(bar); // **main.bar since function's parameter bond to main.bar, **square.kar, **global.tar
  cout << "after forward_square, bar:" << bar << ", &bar:" << &bar << endl; //36
  cout << "after forward_square, kar:" << kar  << ", &kar:" << &kar << endl; //6
  cout << "after forward_square, tar:" << tar  << ", &tar:" << &tar << endl; //36.

  cout << endl << endl << endl;
  forward_plus(bar);// ++main.bar since function's parameter biond to main.bar. ++plus.kar, ++global.tar
  cout << "after_forward_plus, bar:" << bar << ", &bar:" << &bar << endl; //37
  cout << "after forward_plus, kar:" << kar  << ", &kar:" << &kar << endl; //6
  cout << "after forward_plus, tar:" << tar  << ", &tar:" << &tar << endl; //37

  
  return 0;
}
