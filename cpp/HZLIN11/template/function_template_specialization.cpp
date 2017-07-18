#include <iostream>
#include <cstring>
using namespace std;

template <typename T>
int compare(const T& v1, const T& v2) {
    if (v1 < v2) return 1;
    if (v2 < v1) return -1;
    return 0;
}

/*  explicate function will override template.
int compare(const char* const &s1, const char* const &s2) {
    cout << "I am a independent function" << endl;
    return 999;
}
*/

template<>
int compare(const char* const &s1, const char* const &s2) {
    return strcmp(s1, s2);
}

int main() {
    const char *s1 = "hello world";
    const char *s2 = "hi world";
    cout << compare(s1, s2) << endl;
    
    char *s3 = "hey world";
//    cout << compare(s1, s3) << endl; //wrong, function template specialization need accurate matching.
}
