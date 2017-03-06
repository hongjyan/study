#include <iostream>
#include <string>
using namespace std;

int main() {
    string s = "hello world!!!!";

    s.erase(s.end()-4);
    cout << s << endl;

    s.erase(s.begin(), s.begin()+6);
    cout << s << endl;

    s.clear();
    cout << s << endl;

    s = "hello world!!!!";
    
    s.erase(s.size()-6, 6);
    cout << s << endl;
}
