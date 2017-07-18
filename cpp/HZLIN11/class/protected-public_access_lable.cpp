#include <iostream>
using namespace std;

class Base {
public:
    Base(const char *str): name(str) {};
    void output() {
	cout<<"name is "<<name<<endl;
    }
protected:
    string name;
};


class Derived : public Base {
public: 
    Derived(const char *str): Base(str) {}

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

};

int main() {
    Base b("girl");
    Derived d1("lucy");
    Derived d2("lily");
    d1.output();
    d1.get_base_protected_member();
    d1.output_names(d2, b);
}


