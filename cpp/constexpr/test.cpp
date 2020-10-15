#include <iostream>
#include <cstdio>

int main() {
  constexpr int *p = nullptr;
  int i = 0;
  p = &i;
  return 0;
}
