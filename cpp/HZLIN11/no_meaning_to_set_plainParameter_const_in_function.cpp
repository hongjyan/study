#include <iostream>
using namespace std;

void f(const int i, const char c) {
    cout << i << endl;
    cout << c << endl;
}

int main() {
    int i = 1;
    char c = 'c';
    f(i, c);
}    
