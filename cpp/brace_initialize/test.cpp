#include <iostream>
using namespace std;

int foo() {
  return {3.1415};
}

int main() {
  cout << foo() << endl;
  return 0;
}
