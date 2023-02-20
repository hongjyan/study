#include "namespace.hpp"

//2022.10.18 need c++20
namespace has::tracingService::streaming
{
  void f2() {
    f();
    1111 == k ? std::cout<<"correct" : std::cout<<"wrong";
    std::cout << std::endl;
  }

  struct Foo {};
  
  using namespace C98;
  C98::Foo f3(Foo foo)
  { 
    return C98::Foo();
  }

}

int main() {
  std::cout << "C98::i = " << C98::i << std::endl;
  std::cout << "C11::i = " << i << std::endl;
  std::cout << "C11::s is " << s << std::endl;
  std::cout << "global static j is " << j << std::endl;
  has::tracingService::f();
  has::tracingService::streaming::f2();
  return 0;
}
