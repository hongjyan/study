//testing if iterator invalid after inserting element into head/tail of deque. 
//prove iterator maybe invalid if the number of inserted elements exceeds capacity.  
#include <iostream>
#include <deque>
using namespace std;

int main() {
    typedef deque<int>::iterator iter;

    int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    deque<int> i_de(a, a+10);

    iter it1 = i_de.begin() + 4;
    iter it2 = i_de.begin() + 8;
    cout << "*it1 is " << *it1 << endl;
    cout << "*it2 is " << *it2 << endl;

    int head_inc = 0 ;
    int tail_inc = 0;
    
    cout << "please denote how many element will be insert at head:" << endl;
    cin >> head_inc;
    cout << "please denote how many element will be insert at tail:" << endl;
    cin >> tail_inc;

    iter first = i_de.begin();
    for (int i=0; i < head_inc; i++) {
	first = i_de.insert(first, 96);
    }

    iter last = i_de.end();
    for (int i=0; i < tail_inc; i++) {
	last = i_de.insert(last, 69);
    }

    cout << "is it1 still avaiable? *it1 is "  << *it1 << endl; 
    cout << "is it2 still avaiable? *it2 is "  << *it2 << endl; 
}
