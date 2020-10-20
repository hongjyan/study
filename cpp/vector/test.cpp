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


//2020.10.20 test vector assignment by another vector. It is ok to assign bigger vector to lessone and ok vice verse.
    vector<int> vi2 = {1, 2};
    vector<int> vi3 = {1, 2, 3};
    vi3 = vi2;
    for( auto e : vi3) cout << e << " ";
    cout << endl;    

    vector<int> vi4 = {1};
    vi3 = vi4;
    for( auto e : vi3) cout << e << " ";
    cout << endl;    
   
    return 0;
}
