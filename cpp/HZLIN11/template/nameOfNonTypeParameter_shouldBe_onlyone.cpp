#include <iostream>
using namespace std;
template <int N, typename T>
//void foo(T v, int N) {        //N use the same name of non-type parameter. wrong
void foo(T v, int i) {
    cout << v << endl;
    cout << N << endl;
    cout << i << endl;
}

int main() {
//    foo('c', 2);  //wrong, can not deduce N
    foo<3> ('c', 2);
}
