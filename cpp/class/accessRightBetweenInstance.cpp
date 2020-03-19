//instances of same class can access each other's private/protected member, and puiblic member of course.
#include <iostream>
#include <string>
using namespace std;

class Test {
public:
    Test(const char *str, int age): name(str), _age(age) {}

    void output_names(const Test &T) {
        cout<<"my name is "<<name<<endl;
        cout<<"another instance's name is "<<T.name<<endl;
        cout << "my age is " << _age << endl;
        cout << "another instance's age is " << T._age << endl;
    }

protected:
    string name;
private:
    int _age;
};

int main() {
    Test T1("Li Lei", 12);
    Test T2("jim", 14);
    T1.output_names(T2);
}

