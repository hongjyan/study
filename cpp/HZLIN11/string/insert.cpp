#include <iostream>
#include <string>
using namespace std;

int main() {
    string s("hello world");
    cout << s << endl;

    string s2 = "hi, buddy";    

    s.insert(s.begin(), 'H');
    cout << s << endl;
    
    s.insert(s.begin(), s2.begin(), s2.begin()+2);
    cout << s << endl;

    s.insert(s.end(), 10, '!');
    cout << s << endl;

    s = "hello world";
    const char *str = "china finland";

    s.insert(0, 10, '^'); //s.insert(pos1, n, t)
    cout << s << endl;

    s.insert(0, s2);   //s.insert(pos1, s2)
    cout << s << endl;

    s.insert(0, s2, 4, 5);  //s.insert(pos1, s2, pos2, n2)
    cout << s << endl;

//    s.insert(0, s2, 6); //wrong

    s.insert(0, str);  //s.insert(pos1, cp)
    cout << s << endl;

    s.insert(0, str, 5); //s.insert(pos1, cp, n)
    cout << s << endl;
}
