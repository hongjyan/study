/*

    A non-type template-parameter shall have one of the following (optionally cv-qualified) types:

        const integral or enumeration type,
        pointer to object or pointer to function,
        lvalue reference to object or lvalue reference to function,
        pointer to member,
        std::nullptr_t.

*/
#include <iostream>
using namespace std;
template <int N, typename T>
void foo(T v) {
    cout << v << endl;
    cout << N << endl;
}

/*
template <double D>
void foo2(void) {
    cout << D << endl;
}
*/

int main() {
    foo<2>('c');
/* wrong, i is not const
    int i = 3;
    foo<i>('d');
*/

    const int j = 4;
    foo<j>('e');

/* wrong, k is not integer or enum    
    const double k = 5.24;
    foo2<k>();
*/

/* wrong, ‘double’ is not a valid type for a template constant parameter    
    const double d = 3.14;
    foo2<d>();
*/
}
