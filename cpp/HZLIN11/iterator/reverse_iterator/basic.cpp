#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "../../include/output.h"
#include "../../include/trim_string.h"
using namespace std;

int main() {
    string line("first, middle, last");
    typedef string::reverse_iterator r_iter;

    r_iter comma = find(line.rbegin(), line.rend(), ',');
//    string last(comma, line.rbegin() );  //comma++, won't reach line.rbegin().
    string last(line.rbegin(), comma);
    cout << last << endl;

    string las(comma.base(), line.end() );
    cout << las << endl;

//  get words from line.
    vector<string> svec;
    typedef string::const_iterator iter;
    string::size_type pos = 0;
    string::size_type pre = pos;
    while (true) {
	pos = line.find(",", pos);
	string word = line.substr(pre, pos-pre);
	word = trim_string(word);
	svec.push_back(string(word) );
	if ( pos == string::npos) {
	    break;
	}
	else {
	    ++pos;
	    pre = pos;
	}
    }

// sort;
    output(svec);
    sort(svec.begin(), svec.end() );
    output(svec);
    sort(svec.rbegin(), svec.rend() );
    output(svec);
}
