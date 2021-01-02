#include <cstdio>

int main() {
  int &&rref = 10;
  int &ref = rref;
  ref = 12;
  printf("rref is %d, ref is %d, &rref is %p, &ref is %p\n", rref, ref, &rref, &ref);
  return 0;
} 
