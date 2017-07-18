#include <iostream>
#include <vector>
using namespace std;

int main() {
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    vector<int> ivec(a, a+15);
    vector<int>::iterator iter1 = ivec.begin();
    vector<int>::iterator iter2;

//iter + n, iter - n
    iter2 = iter1 + 2;
    cout << "*iter1 is " << *iter1 << endl;
    cout << "*iter2 is " << *iter2 << endl;
    cout << "*(iter2 - 1) is " << *(iter2 - 1) << endl;

//iter1 - iter2
    vector<int>::difference_type size1 = iter2 - iter1;
    vector<int>::size_type size2 = iter2 - iter1;
    int size3 = iter2 - iter1;
    cout << "the difference between iter2 and iter1 is " << size1 << size2 << size3 << endl;

//iter1 += iter2. No += for iter, assume no -= also. 
//    iter2 += iter1;
//    cout << "iter2 dereference value after iter2 += iter1 is " << *iter2 <<endl;

// iter1 >=, <=, >, < iter2
cout << (iter1 <= iter2) << endl;
cout << (iter2 >= iter1) << endl;
    
}
