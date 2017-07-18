#include <iostream>
using namespace std;


template <typename T>
void fref(const T& p, T& q) {}

template <typename T>
void fptr(T* p) {}

void f() {}

template<typename T, size_t N, size_t M>
void array_N(T (&param)[N], T (&param2)[M]) {
    cout << N << endl;
    cout << M << endl;
}

int main () {
    const int i = 3;
    int j = 4;    
    fref(i, j);  //(const, nonconst) apply precisely
//    fref(i, i);  //wrong, can't const->nonconst
    fref(j, j);  //nonconst->const for the first function parameter.

    int a[10];
    fptr(a); //allow array->pointer which points to the first element of array.
    fptr(f); //allow function to pointer.


//no conversion
    int b[10];
    fref(a, b);
    int c[8];
    //fref(a, c); //wrong, fref(int [10], int [8]) not matched.
    
    array_N(a, c);
}
