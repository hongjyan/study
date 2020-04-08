#include <cstdio>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    string s("fun");
    cout << "the address of s is " << &s << endl; //the address of s is 0x7ffec1cc46f0
    string s1(std::move(s));
    cout << "the address of s is " << &s << " after move"  << endl;//the address of s is 0x7ffec1cc46f0 after move
    cout << "the address of s1 is " << &s1 << endl; //the address of s1 is 0x7ffec1cc46e0
    cout << "s is " << s << ", s1 is " << s1 << endl; //s is , s1 is fun
    
    return 0;
} 
