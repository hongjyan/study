#include <iostream>
#include <string>
#include <vector>
#include <functional>

using namespace std;

int main() {
  typedef tuple<size_t, int, string, vector<string>> Foo;
  Foo f; //Foo's values will be default-intialized. {0, 0, string(""), emptyVector} 
  Foo f1(10, 1, "string", {10, "str"});
  Foo f2{10, 1, "string", {10, "str"}};

  auto t = make_tuple(10, 1, "string", vector<string>{10, "str"});  //type same as Foo, but let compiler to defer it.

  typedef decltype(t) t_type;
  const size_t len = tuple_size<t_type>::value;
  if (2 <= len) {
    cout << get<2>(t) << endl;
  }
     
  tuple_element<2, t_type>::type e = get<2>(t); //get copy of the third element from tuple
  cout << e << endl;
 
  //error: the value of 'i' is not usable in a constant expression
  /*
  typedef decltype(t) t_type
  for (size_t i=0; i!=tuple_size<t_type>::value; i++) {
    cout << get<i>(f) << ", ";
  }
  */

  return 0;
}
