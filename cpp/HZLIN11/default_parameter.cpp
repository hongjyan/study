#include <iostream>
using namespace std;

void foo(int a, int b=10) {
    cout<<a<<endl<<b<<endl;
}

void foo2(int, int b=10);
void foo2(int a, int b) {
    cout<<a<<endl<<b<<endl;
}

class Test {
public:
    void output(int a, int b=7);
};

void Test::output(int a, int b) {
    cout<<a<<endl<<b<<endl;
}

class Test2 {
public:
    void output(int a, int b);
};

void Test2::output(int a, int b=7) {
    cout<<a<<endl<<b<<endl;
}

int main() {
    foo(1);
    foo2(2);

    Test T;
    T.output(27);

    Test T2; 
    T2.output(28);
}
