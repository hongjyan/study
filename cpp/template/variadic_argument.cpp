#include <iostream>
#include <ostream>
using namespace std;


//this function must be prior to variadic one below.
template<typename T>
ostream &print(ostream& os, const T &t) {
  return os << t;
}

template<typename T, typename... Args>  //Args is template argument package
ostream &print(ostream &os, const T &t, const Args& ...rest) {  //rest is function argument package
  os << t << ", ";
  return print(os, rest...);
}


//creatived by my own.
template<typename... Args>
ostream &print2(ostream &os, const Args& ...varg) {
  return print(os, varg...);
}


//func(parameter-pack)...
//WOW: iterate parameter-pack, call func for each iterated parameter, so func must be template ifparameters are different type in pack.
//RETURN: list of return vaue of func(each-parameter)

//example 1st
template<typename T>
const T &just_return(const T &t) {
  return t;
}

template<typename... Args>
ostream &print3(ostream &os, const Args& ...varg) {
  return print(os, just_return(varg)...);
}

//example 2nd.
template<typename T>
void print(const T &t) {
  cout << t << " ";
}

template<typename T>
ostream &print_novoid(const T &t) {
  return cout << t << " ";
}

template<typename... Args>
void do_nothing(const Args& ...vargs) { }

template<typename... Args>
void print4(const Args& ...varg) {
  //print(varg)...;  //syntax error. since func(param-pack)... return a list, it must be called be some other function.
  do_nothing( (print(varg), "anything, anytype")... );  //parameter-pack expansion must have something. since print's return type is void, so we need add "anything, anytype"
  cout << endl;
  do_nothing( (print_novoid(varg))... );  //print_novoid return something, so need to add "anything, anytype" 
  cout << endl;
}




int main() {
  int i = 0;
  float flt = 3.14;
  const char *s = "how now brown cow";

  print(cout, i, flt, s);
  cout << endl << endl << endl;

  print2(cout, i, flt, s);
  cout << endl << endl << endl;

  print3(cout, i, flt, s);
  cout << endl << endl << endl;

  print4(cout, i, flt, s);
  cout << endl << endl << endl;
  return 0;
}
