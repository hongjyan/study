//template <class Container, class Iterator>
//  insert_iterator<Container> inserter (Container& x, Iterator it);
#include <iostream>
#include <vector>
#include <deque>
#include <iterator>
#include "../../include/output.h"
using namespace std;

int main() {
    vector<int> i_vec(10, 0);

//    insert_iterator< vector<int> > in;  // no default constructor ofr insert_interator
    inserter(i_vec, i_vec.begin()+5) = 1;
    output(i_vec);

    back_inserter(i_vec) = 2;
    output(i_vec);

    deque<int> i_deq(i_vec.begin(), i_vec.end() );
    front_inserter(i_deq) = 3;
    output(i_deq);

} 
    
