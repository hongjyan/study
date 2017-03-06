#include <iostream>
#include <vector>
using namespace std;

int main() {
    typedef  vector<int>::iterator iter;
    vector<int> i_vec(10, 78);

    iter it = i_vec.begin();

    i_vec.insert(it, 87);
    
    it = i_vec.begin();
    while ( it != i_vec.end() ) {
	cout << *it++ << endl;
    }    
}
