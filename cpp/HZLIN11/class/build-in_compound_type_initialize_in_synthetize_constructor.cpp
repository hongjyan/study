#include <iostream>
using namespace std;

class people {
public:
    void output() {
	if (!name) {
	    cout<<"p.name is NULL"<<endl;
	}
	cout<<"age is "<<age<<", name is "<<name<<endl;
    }
private:
    int age;
    char *name;
};

people P;

int main() {
    people p;
    cout<<"output local p"<<endl;
    p.output();

    cout.clear();

    cout<<"output global P"<<endl;
    P.output();
}
