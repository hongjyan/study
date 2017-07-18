#include <iostream>
using namespace std;

class A {
public:
    A(int v, double d): m_v(v), m_d(d) {}
    A(): m_v(0), m_d(0.0) {}
    int m_v;
    
    double get_double() {
	return m_d;
    }

    void set_double(double d) {
	m_d = d;
    }

/*    A operator*() {
	A* p_temp = new A(m_v, m_d);
	return *p_temp;
    }
*/
private:
    double m_d;
};

void swap(A& r1, A& r2) {
    A& temp = r1;
    r1 = r2;
    r2 = temp;
    cout << "temp.m_v is " << temp.m_v << endl;
}

void swap2(A v1, A v2) {
    A temp = v1;
    v1 = v2;
    v2 = temp;
}

void swap(A* p1, A* p2) {
    A temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}


int main() {
    A A1(1,1.0);
    A A2(2, 2.0);
    cout << "A1.m_v is " << A1.m_v << ", A2.m_v is " << A2.m_v << " before swap." << endl;
    swap(A1, A2);
    cout << "A1.m_v is " << A1.m_v << ", A2.m_v is " << A2.m_v << " after swap." << endl;

    A A5(5, 5.0);
    A A6(6, 6.0);
    cout << "A6.m_v is " << A1.m_v << ", A6.m_v is " << A2.m_v << " before swap." << endl;
    swap2(A5, A6);
    cout << "A6.m_v is " << A1.m_v << ", A6.m_v is " << A2.m_v << " after swap." << endl;
    
    A A7(7, 7.0);
    A A8(8, 8.0);
    cout << "A8.m_v is " << A8.m_v << ", A8.m_v is " << A8.m_v << " before swap." << endl;
    swap(&A7, &A8);
    cout << "A8.m_v is " << A8.m_v << ", A6.m_v is " << A8.m_v << " after swap." << endl;




    A A3(3, 3.0);
    A A4(4, 4.0);
    A* p = &A3;
    cout << "Memory address of A3 is " << &A3 << " at first" << endl;
    cout << "Memory address of A4 is " << &A4 << " at first" << endl;
    cout << "A3.m_v is " << A3.m_v << " at first" << ", A3.m_d is " << A3.get_double() << " at first" << endl;
    A& r = A3;
    r = A4;
    cout << "A3.m_v is " << A3.m_v << " later" << ", A3.m_d is " << A3.get_double() << " later" << endl;
    cout << "Memory address of A3 is " << &A4 << " later" << endl;
    cout << "p is " << p << endl; cout << "(*p).m_v is " << (*p).m_v << ", (*p.m_d) is " << (*p).get_double() << endl;
}

