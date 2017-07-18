#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include "../../include/output.h"
using namespace std;

int main() {
    istream_iterator<int> eof;

    cout << "please input integer, non-integer to stop" << endl;
    istream_iterator<int> it(cin);
    vector<int> ivec(it, eof);
    output(ivec);
    
    ostream_iterator<int> output(cout, " ");
    sort(ivec.begin(), ivec.end() );
    unique_copy(ivec.begin(), ivec.end(), output);

}
