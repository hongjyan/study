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
    cout << endl << endl << endl;

   
    cout << "std::move  just cast lval to rval, no more" << endl;
    vector<int> ivec{1, 2, 3, 4};
    int i = std::move(ivec)[0];
    cout << "i is " << i << ", ivec[0] is " << ivec[0] << endl;
    
    vector<int> ivec2(std::move(ivec));
    cout << "ivec2[0] is " << ivec2[0] << ", ivec.size() is " << ivec.size() << endl;   

    return 0;
} 
