#include <vector>
#include <iostream>

using namespace std;


int main() {
//2017.3.4 test iterator move when erasing.
    int array[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    vector<int> ivec{array, array + sizeof(array)/sizeof(int)};
    for (vector<int>::iterator it = ivec.begin(); it != ivec.end(); ++it) {
        ivec.erase(it);
    }
    for (vector<int>::iterator it = ivec.begin(); it != ivec.end(); ++it) {
        cout << *it << ", ";
    }
    cout << endl;

    for (vector<int>::iterator it = ivec.begin(); it != ivec.end(); ++it) {
        ivec.erase(it);
        --it; 
    }
    for (vector<int>::iterator it = ivec.begin(); it != ivec.end(); ++it) {
        cout << *it << ", ";
    }
    cout << endl;
//endl
    return 0;
}
