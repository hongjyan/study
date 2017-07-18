//this pro is used to test way to intialize a string
#include <iostream>
#include <string>
using namespace std;

int main() {
    string s;

/* wrong
    string s1(10);
    cout << s1.size() << endl;
    cout << s1 << endl;
*/

    string s2(10,'c');  //s(n, t)
    cout << s2 << endl;

    string s3(s2.begin(), s2.begin()+5); //s(b, e)
    cout << s3 << endl;
    
    string s4(s3);  //s(s2)
    cout << s4 << endl;

    string s5 = "hello world";  //s(s2)
    cout << s5 << endl;

    string s6("C++");   //s(cp)
    cout << s6 << endl;

    const char *str = "c plus plus";  
    string s7(str);    //s(cp)
    cout << s7 << endl;

    string s8(str, 6);  //s(cp, n)
    cout << s8 << endl; 

    string s9(s7, 2);  //s(s2, pos)
    cout << s9 << endl;

    string s10(s7, 2, 4); //s(s2, pos, n)
    cout << s10 << endl;
}
