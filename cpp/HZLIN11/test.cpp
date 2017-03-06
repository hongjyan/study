#include <iostream>
#include <cstring>
#include <cstdio>

using namespace std;

int main() {
    int a[] = {1, 2, 3, 4};
    char str[] = {'a', 'b', 'c'};
    const char *s = "fun";
    char *p_new = new char[4];
    strcpy(p_new, "abc");
    s = p_new;


    cout << a << endl;
    cout << str << endl;
    cout << s << endl;
    cout << p_new << endl;

//2017.1.22
    printf("sizeof(int) is %lu\n", sizeof(int));
    printf("sizeof(unsigned long) is %lu\n", sizeof(unsigned long));
    return 0;
}
