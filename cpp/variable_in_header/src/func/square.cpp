#include "common.hpp"
#include <iostream>

int afterward_square(const int i) { 
  return i*i;
}

int forward_square(int &i) {
  std::cout << "<<<<<<< forward_square start >>>>>>>>>>>" << std::endl;

  i = i*i;  //i
  kar = kar*kar; //square.kar
  tar = tar*tar; //global.tar

  std::cout << "bar in forward_square:" << bar << ", &bar:" << &bar << std::endl; //6, square.bar
  std::cout << "kar in forward_square:" << kar << ", &kar:" << &kar << std::endl; //36.
  std::cout << "tar in forward_square:" << tar << ", &tar:" << &tar << std::endl; //unknown

  std::cout << "<<<<<<< forward_square end >>>>>>>>>>>" << std::endl;
  return i;
}
