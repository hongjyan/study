#include <iostream>

/*********** inline namespace *********************/
namespace C98 {
  int i = 10;
}

inline namespace C11 {  //let C11 as default namespace.
  int i = 11;
}

//static int i = 12;  //ambiguous with inline C11::i


namespace C11 {  //no need to specify inline again.
  const char *s = "hello world";
}


/************** unnamed namespace vs static *******************/
/**** see https://stackoverflow.com/questions/154469/unnamed-anonymous-namespaces-vs-static-functions ****/
//static int j = 13;  //OK, c98 style
namespace {     //OK, c11 style
  int j = 13;
}


int main() {
  std::cout << "C98::i = " << C98::i << std::endl;
  std::cout << "C11::i = " << i << std::endl;
  std::cout << "C11::s is " << s << std::endl;
  std::cout << "global static j is " << j << std::endl;
  return 0;
}
