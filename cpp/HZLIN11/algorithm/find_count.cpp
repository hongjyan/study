#include <iostream>
#include <deque>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    deque<int> i_deq(a, a+7);
    typedef deque<int>::iterator it;
    string s_array[] = {"game", "test", "fun", "hello"};
    vector<string> svec(s_array, s_array+4);


//find
    it iter = find(i_deq.begin(), i_deq.end(), 5);
    if (i_deq.end() != iter) {
	cout << "find " << *iter << endl;
    }
    else {
	cout << "not found" << endl;
    }

    cout << *(find(svec.begin(), svec.end(), "hello") ) << endl;



//    cout << *(find(svec.begin(), svec.end(), "hel") ) << endl; //will cause segementation fault, since can't  dereference v.end().

//count
    i_deq.push_front(3);
    cout << count(i_deq.begin(), i_deq.end(), 3) << endl;
    cout << count(a, a+9, 9) << endl;
} 
