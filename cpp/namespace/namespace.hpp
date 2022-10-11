#include <iostream>

int k = 10;

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

//2022.10.18
namespace has
{
  int k = 1111;
namespace tracingService
{
  void f() {
    std::cout << "k is " << k << ", it should be 1111 since it is the value \
      of innerMost namespace" << std::endl;
    std::cout << "C11::i is " << C11::i << std::endl; //prove global namespace will be searched for namespace too. 
  } 
}
}
