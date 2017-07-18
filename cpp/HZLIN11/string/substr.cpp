#include <iostream>
#include <string>
using namespace std;

int main() {
    string s = "hello world!!!!";
    
    cout << s.substr(s.size()-9, 9) << endl;
    
    cout << s.substr(6) << endl;
}
