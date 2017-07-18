#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include "../include/output.h"
#include <iterator>
using namespace std;

int main() {
    string str_array[] = {"test", "a", "fun", "world"};
    vector<string> svec(str_array, str_array+4);
    reverse(svec.begin(), svec.end());
    output(svec);

    vector<string> r_svec;
    reverse_copy(svec.begin(), svec.end(), back_inserter(r_svec) );
    output(r_svec);
    output(svec);
}

