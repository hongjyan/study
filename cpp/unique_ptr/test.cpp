#include <iostream>
#include <memory>
using namespace std;

int main() {
  typedef int arry10[10];
  unique_ptr<int[]> up(new arry10{1, 2, 3, 4}); //recommend way to create weak_ptr
  
  int *p = new arry10{11, 22, 33};
  unique_ptr<int[]> up2(p);

  up2.release();
  /* test if up.release has release dynamic array or not */
  cout << *(p+1) << endl; //coredump if up2.release() do release memory, but seems not.
  
  return 0;
}
