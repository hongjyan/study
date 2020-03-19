#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include "../include/output.h"
using namespace std;

int main() {
    int a[] = {2, 3, 5, 7, 12, 15, 2, 8};
    vector<int> ivec(10, 77);
    vector<int> mvec;
    
    merge(a, a+7, ivec.begin(), ivec.end(), back_inserter(mvec) );
    output(mvec);
}
