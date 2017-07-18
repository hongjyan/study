#include <iostream>
#include "../include/output.h"
using namespace std;

template <size_t N, typename T>
void initialize_mutable_len_array(T* a) {
    for(int i=0; i<=N; ++i) {
	a[i] = i;
    }
}


int main() {
    int a[100];
    initialize_mutable_len_array<100>(a);
    for(int i=0; i<= sizeof(a)/sizeof(int); ++i) {
	cout << a[i] << " ";
    }
    cout << endl;
}	
