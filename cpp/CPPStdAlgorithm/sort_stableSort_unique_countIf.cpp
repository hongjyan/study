#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <algorithm>
#include "../include/output.h"
using namespace std;

bool isShorter(const string &s1, const string &s2) {
    return (s1.size() < s2.size() );
}

struct GT {
    GT(int base_size): m_base_size(base_size) {}

    bool operator()(const string &s) {
	return (s.size() >= m_base_size);
    }

    int m_base_size;
};

int main() {
    string s_array[] = { "the", "quick", "red", "fox", "jumps", "over", "the", "slow", "red", "turtle" };
    vector<string> s_vec(s_array, s_array+10);
    typedef vector<string>::iterator iter;
    deque<string> s_deq;

    output(s_vec);

    
    sort(s_vec.begin(), s_vec.end() );
    output(s_vec);

    unique_copy(s_vec.begin(), s_vec.end(), back_inserter(s_deq));
    output(s_deq);    

    sort(s_vec.begin(), s_vec.end(), isShorter); //equivalent elements are not guaranteed to keep their original order
    stable_sort(s_vec.begin(), s_vec.end(), isShorter); //preserves the relative order of the elements whith equivalent values
    output(s_vec);


    iter it = unique(s_vec.begin(), s_vec.end());
    output(s_vec);

    s_vec.erase(it, s_vec.end() );
    output(s_vec);

    int count = count_if(s_vec.begin(), s_vec.end(), GT(6));
    cout << "There are " << count << " words which size is bigger than 6" << endl;
}
