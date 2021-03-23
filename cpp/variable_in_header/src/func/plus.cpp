#include "common.hpp"
#include <iostream>

int afterward_plus(const int i) {
  return i+1;
}

int forward_plus(int &i) {
  std::cout << "<<<<<<<<<<<<<<< forward_plus start >>>>>>>>>>>>>>>>" << std::endl;

  ++i;
  ++kar; //plus.kar
  ++tar; //global.tar

  std::cout << "bar in forward_plus:" << bar  << ", &bar: " << &bar << std::endl; //6, plus.bar
  std::cout << "kar in forward_plus:" << kar  << ", &kar: " << &kar << std::endl; //7, plus.kar
  std::cout << "tar in forward_plus:" << tar  << ", &tar: " << &tar << std::endl; //unknow.

  std::cout << "<<<<<<<<<<<<<<< forward_plus end >>>>>>>>>>>>>>>>" << std::endl;
  return i;
}
