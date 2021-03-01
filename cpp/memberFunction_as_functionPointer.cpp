#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int main() {
  vector<string> svec = {"1st", "2nd", "3rd", ""};
  auto fp = &string::empty;
  //cout << *find_if(svec.begin(), svec.end(), fp) << endl;

  //function<proto type of member function>
  function<bool (const string &)> fcn = &string::empty;
  cout << *find_if(svec.begin(), svec.end(), fcn) << endl;

  //mem_fn
  cout << *find_if(svec.begin(), svec.end(), mem_fn(&string::empty)) << endl;
  auto f = mem_fn(&string::empty);
  cout << f(svec[0]) << endl;  //ok to take object 
  cout << f(&svec[0]) << endl; //ok to take pointer to object

  //bind
  cout << *find_if(svec.begin(), svec.end(), bind(&string::empty, _1)) << endl; //the first parm of &string::empty is _1, so it is ok to cal _1.empty() when _1 is object, _1->empty() when _1 is pointer. here _1 is object.
  auto f2 = bind(&string::emtpy, _1);
  cout << f2(svec[0]) << endl;
  cout << f2(&svec[0]) << endl;
  
  
  return 0;
}
