#include <iostream>
#include <string>
using namespace std;

int main() {
    string s = "hello world!!!";
    string s2 = s.substr(6,5);
    const char *str = "hello";
    
    cout << string::npos << endl;

//arg: (c, [pos]), (s, [pos]), (cp, [pos]), (cp, pos, n)
//find
    cout << s.find('!') << endl;
    cout << s.find('!', 0) << endl;

    cout << s.find(s2) << endl;
    cout << s.find(s2, 7) << endl;

    cout << s.find(str) << endl;
    cout << s.find(str, 1) << endl;

    cout << s.find(str, 0, 3) << endl;

cout << "--------------------" << endl;
//rfind, when pos is specified, ignoring any possible match beginning after pos. 
    cout << s.rfind('!') << endl;  //start searching from the last index: string::npos.
    cout << s.rfind('!', 0) << endl;  //starting from pos:0 to find in reverse direction

    cout << s.rfind(s2) << endl;
    cout << s.rfind(s2, 5) << endl;
    cout << s.rfind(s2, 6) << endl;

    cout << s.rfind(str) << endl;
    cout << s.rfind(str, 4) << endl;

    cout << s.rfind(str, 0, 3) << endl;

cout << "------------------------" << endl;
//find_first_not_of(args), find the first index of any character of args which occurs in s. 
    cout << s.find_first_of('!') << endl;
    cout << s.find_first_of('!', 12) << endl;

    cout << s.find_first_of("lord") << endl;
    cout << s.find_first_of("lord", 6) << endl;

    cout << s.find_first_of(s2) << endl;
    cout << s.find_first_of(s2, 6) << endl;

    cout << s.find_first_of(str, 2, 3) << endl;

    cout << s.find_first_not_of("hellow") << endl;


cout << "-------------------------" << endl;
//find_last_of(args), searching the string for the last characters that matches any of the characters specified in its arguments
//when pos is specified, the search only includes characters at or before position pos, ignoring any possible occurrences after pos
//searching in reverse direction
    cout << s.find_last_of('!') << endl;
    cout << s.find_last_of('w', 5) << endl;
    
    cout << s.find_last_of("lord") << endl;
    cout << s.find_last_of("lord", 1) << endl;

    cout << s.find_last_of(str, 4, 2) << endl;

    cout << s.find_last_of(s2) << endl;
    cout << s.find_last_of(s2, 4) << endl;


    cout << s.find_last_not_of("hellow") << endl;

}
