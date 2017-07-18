#include <iostream>
#include <vector>
using namespace std;

int main() {
    const int size = 10;
    int size2;
    cout << "please input a integer:";
    cin >> size2;
    vector<int> i_v(size2);
    vector<int> i_vec(size+size2, 0);
    for(vector<int>::iterator iter=i_vec.begin(); iter<i_vec.end(); iter++) {
	cout<<*iter<<endl;
    }
} 
