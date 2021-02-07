#include <cstdio>

void fcn2() {
  int v1 = 42;
  auto f = [ v1 ] () mutable { return ++v1; };
  printf("v1 is %d after lambda defined\n", v1);
  auto v2 = f();
  printf("lambda return is %d\n", v2);
  printf("v1 is %d after lambda called\n", v1);
}

int main(int argc, char* argv[]) {
  fcn2();
} 
