#include <iostream>
#include <string>
using namespace std;

template<typename T>
class tem {
public:
    void output() {
	cout << "I am general function in class template" << endl;
    }

    void speak() {
	cout << "whoo" << endl;
    }
};

//only specialize one member of class template
template<>
void tem<int>::output() {
    cout << "I am specilized one" << endl;
}
//specialized class template won't inherite anything from general class template.

//specialize whole class template
template<>
class tem<string> {
public:
    tem(const string& s);
    
    void output() {
	cout << mstr << endl;
    }
private:
    string mstr;
};

tem<string>::tem(const string &s): mstr(s) {} //template<T> shall not been added at the beginning for member function of specialized class\
						 than the normal member does.
//specialize partial class template
template <typename T1, typename T2>
class CLS {
public:
    CLS(const T1 &, const T2 &);

    void output() {
	cout << "I am in general CLS" << endl;
	cout << m_v1 << endl;
	cout << m_v2 << endl;
    }

private:
    T1 m_v1;
    T2 m_v2;
};

template <typename T1, typename T2>
CLS<T1, T2>::CLS(const T1 &v1, const T2 &v2): m_v1(v1), m_v2(v2) {}


template <typename T1>
class CLS<T1, int> {
public:
    CLS(const T1 &, int);

    void output() {
        cout << "I am in partial specialized class template CLS" << endl;
        cout << m_v1 << endl;
        cout << m_v2 << endl;
    }   

private:
    T1 m_v1;
    int m_v2;  
};

template <typename T1>
CLS<T1, int>::CLS(const T1& v1, int v2): m_v1(v1), m_v2(v2) {}


int main() {
    tem<double> T1;
    T1.output();
    T1.speak();

    tem<int> T2;
    T2.output();
    T2.speak();

    tem<string> T3("fun");
    T3.output();
//    T3.speak();  //Wrong, tem<string> is whole specialized class template, won't inherate any member from general class template. 
    
    CLS<int, string> c1(1, "fun");
    c1.output();
    
    CLS<string, int> c2("fun", 1);
    c2.output();
}
