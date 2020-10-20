#include <cstdio>
#include <initializer_list>
using namespace std;

int main() {
  initializer_list<int> il;
  il = {1, 2, 3, 4};
  printf("il.size() is %lu\n", il.size());

  initializer_list<int> il2{1, 2, 3, 4, 5};
  printf("il2.size() is %lu\n", il2.size());

  il = il2;
  auto il4(il);
  printf("il.size() is %lu, il2.size() is %lu, il4.size() is %lu\n", il.size(), il2.size(), il4.size());


  int i=1, j=2, k=3;
  auto il3 = {i, j, k}; //use copy which is const of i,j,k to initialize il3
  for (auto e : il3) printf("%d ", e);
  printf("\n");
  i=11, j=22, k=33;
  for (auto e : il3) printf("%d ", e); //so, we cann't change content of il3 via i,j,k
  printf("\n");
  auto it = il3.begin();
  // *it = 22; //error: assignment of read-only location '* it'
  

  //initialize_list share element when one is assigned by another
  {
    auto il5 = { 1, 2, 3, 4, 5};
    il3 = il5;
  }
  for (auto e : il3) printf("%d ", e); //so, we cann't change content of il3 via i,j,k
  printf("\n");

  return 0;
}
