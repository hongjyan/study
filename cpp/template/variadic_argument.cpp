#include <iostream>
#include <ostream>
using namespace std;


//this function must be prior to variadic one below.
template<typename T>
ostream &print(ostream& os, const T &t) {
  return os << t;
}

template<typename T, typename... Args>  //Args is template argument package
ostream &print(ostream &os, const T &t, const Args... rest) {  //rest is function argument package
  os << t << ", ";
  return print(os, rest...);
}




int main() {
  int i = 0;
  float flt = 3.14;
  const char *s = "how now brown cow";
  print(cout, i, flt, s);
  cout << endl << endl << endl;


  return 0;
}
