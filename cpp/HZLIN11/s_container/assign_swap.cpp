#include <iostream>
#include <vector>
#include "output.h"
using namespace std;

int main() {
    typedef vector<int>::iterator iter;
    int a[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    int b[10] = {11, 22, 33, 44, 55, 66, 77, 88, 99, 100};
    vector<int> vec_a(a, a+8);
    vector<int> vec_b(b, b+10);
    output(vec_a);
    output(vec_b);

//assignment using  =    
    vec_a = vec_b;
    output(vec_a);
    
//assignment using assign
    vec_a.assign(vec_b.begin(), vec_b.end()-2);
    output(vec_a);
    
//swap
    vec_a.assign(a, a+8);
    iter it_a = vec_a.begin() + 5; 
    iter it_b = vec_b.begin() + 5;
    cout << "*it_a is " << *it_a << endl;
    cout << "*it_b is " << *it_b << endl;

    vec_a.swap(vec_b);

    cout << "after swap, *it_a is " << *it_a << endl;
    cout << "after swap, *it_b is " << *it_b << endl;

    cout << "after swap, vec_a is " << endl;
    output(vec_a);
    cout << "after swap, vec_b is " << endl;
    output(vec_b);
}
