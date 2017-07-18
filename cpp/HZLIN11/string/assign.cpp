//ways to assign string
#include <iostream>
#include <string>
using namespace std;

int main() {
    string s;
    s.assign(10, 'a');
    cout << s << endl;
    
    string s1 = "hello world";
    cout << s1 << endl;

    s1.swap(s);
    cout << s << endl;
    cout << s1 << endl;

    s.assign(s1.begin(), s1.begin()+6);
    cout << s << endl;

    const char *str = "nokia finland oulu";
    s.assign(str);
    cout << s << endl;

    s.assign(str, 5);
    cout << s << endl;

    s.assign(s1);
    cout << s << endl;

//    s.assign(s1, 6);
    s.assign(s1, 6, 12);
    cout << s << endl;
}
