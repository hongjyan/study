#include <iostream>
using namespace std;

template <typename T, size_t N>
class foo {
public:
    foo(const T& v): m_v(v) {}

    void output() {
	cout << m_v << endl;
	cout << N << endl;
    }

private:
    T m_v;
};

int main() {
    double d = 13.21;
    foo<int, 12> obj(d);
    obj.output();  
}
