#include <iostream>
using namespace std;

class Base {
public:
    Base(int age, const char *str): m_age(age), name(str) {};
    
    virtual void output() {
	cout<<"name is "<<name<<endl;
    }
    int m_age;
protected:
    string name;
};


class Derived : protected Base {
public: 
    Derived(int age, const char *str): Base(age, str) {}
    
    virtual void output() {
	cout<<"name is "<<name<<endl;
	cout<<"age is "<<m_age<<endl;
    }

    string get_base_protected_member() {
	cout<<"Base::name is "<<Base::name<<endl;  //can access its own base class's member, accutually, \
						    Base::name == this.name
	return Base::name;
    }

    void output_names(const Derived &d, const Base &b) {
	cout<<"my own name is "<<name<<endl; 
	cout<<"name of other derive class is "<<d.name<<endl; //very interesting, can access other derived \
								class's member.
//	cout<<"name of other Base class is "<<b.name<<endl;  //can't access other base class's protected \
								member surelly. 
    }

    void output_ages(const Derived &d, const Base &b) {
	cout<<"my neighbour age is "<<d.m_age<<endl;
	cout<<"The age of father of another neighbour is "<<b.m_age<<endl;
    }
};

int main() {
    Base b(0, "girl");
    Derived d1(13, "lucy");
    Derived d2(11, "lily");
    d1.output();
    d1.get_base_protected_member();
    d1.output_names(d2, b);
    d1.output_ages(d2, b);
}


