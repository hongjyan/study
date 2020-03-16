#include <cstdio>
#include <iostream>
using namespace std;

enum sex {
    MALE, 
    FEMALE
};

class People {
public:
    People(int age, const char *name, sex sx): _age(age), _name(name), _sex(sx) {};
    
    virtual void print() {
        printf("my name is %s, I am %d years old, I am a %s\n", _name.c_str(), _age, _sex ? "female":"male");
    }

    sex getSex() { return _sex; }

public:
    int _age;

protected:
    string _name;

private:
    sex _sex;
};


class Employee : protected People {
public: 
    Employee(int age, const char *name, sex sx, int salary): People(age, name, sx) {
        _salary = salary;
    }
    
    virtual void print() {
        cout << endl;
        People::print();
        printf("my salaray is %d\n", _salary);

	cout<<"People::name is "<<&(People::_name)<<endl;  //acutally, People::name is this.name
        cout << "this::name is " << &_name << endl;

	cout<<"People::age is "<<&(People::_age)<<endl;
        cout << "this::age is " << &_age << endl;

        //cout << "People::sex is " << People::_sex << endl; //error: 'sex People::_sex' is private
        //cout << "this::sex is " << _sex << endl; //error: 'sex People::_sex' is private. _sex do exist but it can not be access by derived class since it is private variable in base class
        cout << "this::sex is " << this->getSex() << endl; //prove _sex exist.

        cout << "this::salary is " << _salary << endl; 
    }


    void print(const Employee &d, const People &b) {
	cout<<"another employee's name is "<<d._name<<endl; //can access Derived class's protected member no matter it belongs to own or other instance.
	//cout<<"another people's name is  "<<b._name<<endl;  //can't access base class's protected member surelly. 
        cout << "another employee's salary is " << d._salary << endl; //as protected member, private member can be accessed by other instance also
    }

private:
    int _salary;
};

int main() {
    cout << "------------------" << endl;
    People p(10, "jim", MALE);
    p.print();

    cout << "------------------" << endl;
    Employee e1(23, "lucy", FEMALE, 10000);
    e1.print();
    
    cout << "------------------" << endl;
    Employee e2(21, "lily", FEMALE, 20000);
    e1.print(e2, p);
}

