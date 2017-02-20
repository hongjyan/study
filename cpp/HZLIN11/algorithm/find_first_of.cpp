//The iterator types in each pair must match exactly, but there is no requirement that the type of the two pairs match each other. In particular, the elements can be stored in different kinds of sequences. What is required is that we be able to compare elements from the two sequences.
//
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int a[] = {1, 2, 3, 4, 5, 6};
    char c[] = {'1', '2', '3', '4'};
//    int c[] = {1, 2, 3, 4};
    vector<int> i_vec(a, a+6);
    vector<char> c_vec(c, c+4);
    typedef vector<int>::iterator i_iter;

    i_iter i_it = i_vec.begin();
    int count = 0;

    cout << ('3' == 3) << endl;


/*
    while (i_it != i_vec.end() ) {
	i_it = find_first_of(i_it, i_vec.end(), c_vec.begin(), c_vec.end() );
	++i_it;
	++count;
	cout << *i_it << endl;
    }
*/

    while (true) {
	i_it = find_first_of(i_it, i_vec.end(), c_vec.begin(), c_vec.end() );
	if (i_it == i_vec.end() ) {
	    break;
	}
	cout << *i_it << endl;
	++i_it;
	++count;
    }

    cout << "find " << count << " match" << endl;
}


