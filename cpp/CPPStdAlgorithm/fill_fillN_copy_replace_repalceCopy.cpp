#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include "../include/output.h"
using namespace std;

int main() {
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    vector<int> ivec(a, a+5);
    deque<int> ideq;
    vector<int> ivec2;

    fill(ivec.begin(), ivec.end(), 11);
    output(ivec);

//    fill_n(ivec.begin(), 10, 22); //10 exceed size of ivec, will cause diaster during run-time
    fill_n(back_inserter(ivec), 10, 22);
    output(ivec);

    copy(a, a+9, front_inserter(ideq) );
    output(ideq);
    copy(a, a+9, inserter(ideq, ideq.begin() ) );
    output(ideq);

    replace(ideq.begin(), ideq.end(), 9, 99);
    output(ideq);

    replace_copy(ideq.begin(), ideq.end(), back_inserter(ivec2), 8, 88);
    output(ideq);
    output(ivec2);
}
 
