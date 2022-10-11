#include "namespace.hpp"

//2022.10.18 need c++20
// namespace has::tracingService::streaming
// {
//   void f2() {
//     std::cout << "k is " << k << ", it should be 1111 since it is the value \
//       of innerMost namespace" << std::endl;
//     std::cout << "C11::i is " << C11::i << std::endl; //prove global namespace will be searched for namespace too. 
//   } 
// }

int main() {
  std::cout << "C98::i = " << C98::i << std::endl;
  std::cout << "C11::i = " << i << std::endl;
  std::cout << "C11::s is " << s << std::endl;
  std::cout << "global static j is " << j << std::endl;
  has::tracingService::f();
  // has::tracingService::f2();
  return 0;
}
