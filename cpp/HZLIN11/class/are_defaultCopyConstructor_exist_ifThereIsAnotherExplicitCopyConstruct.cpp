#include <iostream>
using namespace std;

class bar {
private:
    int m_a;
    double m_b;
//    int m_array[10];
public:
    bar(int a):m_a(a) {}
    int get() {
	return m_a;
    }
};

class foo {
private:
    int m_c;
    bar m_bar;
    int *p;

public:
    foo(int a): m_bar(a), p(new int(0)) {}

    foo(foo& orig): m_c(orig.m_c), m_bar(orig.m_bar), p(new int(2)) {
	cout << "I am not the synthesized one" << endl;
    }
	
    void output() {
	cout << "*p is " << *p << ", m_c is " << m_c << ", private:m_bar.m_a is " << m_bar.get() << endl;
    }
    
    ~foo() {
	delete p;
    }
};

int main() {
    foo f(1);
    foo f2(f);
    f2.output();
}
