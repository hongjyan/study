#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>

using namespace std;

int main() {
    vector<int> ivec;
    std::string foo("foo");
//2017.3.20, will overflow to assign big value
    char c = 12222;  //overflow
    int i = 12222;
    c = i; // not overflow

//2017.3.22 
    cout << sizeof(long) << endl;
//2017.3.28
    unsigned int j = atoi("game");
    cout << j << endl;
    j = atoi("-1");
    cout << j << endl;
    return 0;
}
