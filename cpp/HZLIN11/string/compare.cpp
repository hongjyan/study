#include <iostream>
#include <string>
#include <cstring>
using namespace std;

int main() {
    string s1 = "hello world!!!";
    string s2 = "hey world!!!";
    const char *str = "hi world!!!";

    cout << s1.compare(s2) << endl;
    
    cout << s1.compare(6, s1.size()-6, s2) << endl;

    cout << s1.compare(6, s1.size()-6, s2, 4, s2.size()-4) << endl;
    
    cout << s1.compare(str) << endl;

    cout << s1.compare(6, s1.size()-6, str) << endl;

    cout << s1.compare(0, 5, str, 2) << endl; 

}
