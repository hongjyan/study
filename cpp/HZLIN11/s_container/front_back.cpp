#include <iostream>
#include <vector>
using namespace std;

int main() {
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8};
    vector<int> ivec(a, a+8);

    int val1 = *ivec.begin();
    int val2 = ivec.front();
    vector<int>::reference r_val1 = ivec.front();
    cout << val1 << " " << val2 << " " << r_val1 << endl;

    int val3 = *--ivec.end();
    int val4 = ivec.back();
    vector<int>::reference r_val2 = ivec.back();
    cout << val3 << " " << val4 << " " << r_val2 << endl;

    ++val2;
    cout << ivec[0] << endl;
    ++ivec.front();
    cout << ivec[0] << endl;
    ++r_val1;
    cout << ivec[0] <<endl;

    ++val4;
    cout << ivec[7] << endl;
    ++ivec.back();
    cout << ivec[7] << endl;
    ++r_val2;
    cout << ivec[7] << endl;
    
//de-reference empty pointer will cause segement fault in run-time.
    vector<char> cvec;
    int *p = NULL;
    cout << *p << endl;
    cout << cvec.front() << endl;
    cout << *cvec.begin() << endl;
}
