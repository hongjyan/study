#include <iostream>
#include <typeinfo>
#include <string>
using namespace std;

class Base {
public:
    Base();   
    char *get_name();
    char *set_name(const char *str);
    virtual void output();

protected:
    int get_age();

private:
    int age;
    const char *name;
};


class Derived : public Base {
public:
    Derived();       
    void output();
    char *set_sex(const char *s);

/* comment since derived class can't access base's private member no matter what inheritate lable is.
    int IncreaseAge() {
	age++;
	return age;
    }
*/

private:
//    const char *sex;  //if there is const char * data member 
    string sex;
};


Base::Base():name(""), age(0) {};

char* Base::set_name(const char *str) {
    name = str;
}

void Base::output() {
    cout<<"age is "<<age<<", name is "<<name<<endl;
    if (cout.bad()) {
	cout.clear();
	cout<<"\nclear badbit done"<<endl;
    }

    if (!name) {
	cout<<"name is null pointer, will cause output afterward disappear"<<endl;
    }
}



//Derived::Derived(): Base(), sex(NULL) {};   //cout NULL pointer if this constructor in use, that will\
						lead error of ostream which cause no print out afterward \
						disappear
 
//Derived::Derived(): Base(), sex("") {};
Derived::Derived(): Base() {};

char* Derived::set_sex(const char *s) {
    sex = s;
}

void Derived::output() {
    Base::output();
    cout<<"hello"<<endl;
    cout<<"sex is "<<sex<<endl;
    cout<<"hello"<<endl;
}

Derived D;

int main() {
    cout<<"output global D"<<endl;
    D.output();
    Derived d;
//    d.IncreaseAge();
    cout<<"output local(in main) d"<<endl;
    d.output();
    d.set_name("joe");
    d.set_name("chandler");
    d.set_sex("male");
    cout<<"output local(in main) d again after settig name&sex"<<endl;
    d.output();
}
