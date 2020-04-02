#include <cstdio>
#include <string>
#include <iostream>

using namespace std;

int main() {
    string s("fun");
    string s1(std::move(s));
    cout << "s is " << s << ", s1 is " << s1 << endl;
    cout << "sizeof(s) is " << sizeof(s) << endl;
    cout << "sizeof(s1) is " << sizeof(s1) << endl;
    s1 = "happy";
    cout << "sizeof(s1) is " << sizeof(s1) << endl;
    
    return 0;
} 
