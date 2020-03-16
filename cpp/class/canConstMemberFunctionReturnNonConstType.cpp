#include <stdio.h>
#include <string>
//In a const-function, every data member becomes const in such way that it cannot be modified.

class Person {
public:
    Person(std::string _name, int _age, int _score): _name(_name), _age(_age), _score(_score) { _salary = new int(0); }

    const std::string& getName_good() const {  //OK
        return _name;
    }

//error: invalid initialization of reference of type 'std::string& {aka std::basic_string<char>&}' from expression of type 'const string {aka const std::basic_string<char>}'
//_name in const function is taken as const since const function return value is supposed to be not changable. 
/*
    std::string& getName_error() const {
        return _name;
    }
*/

//Right. getAge() return value, value-assignment is always copy not itself. 
    int getAge() const {                   
        return _age;
    }

//error: invalid initialization of reference of type 'int&' from expression of type 'const int' 
/*
    int & getScore() const {
        return _score;
    }
*/

//seems wrong, but it is right. Since const here requires _salary can not be changed rather than the content _salary points to
    int* getSalary() const {
        return _salary;
    }

    ~Person() {
        delete _salary;
    }
private:
    std::string _name;
    int _age;
    int _score;
    int *_salary;
};


void myCode(const Person& p)  // myCode() promises not to change the Person object...
{
    int *pointer = p.getSalary();
    printf("p.getSalary() is %d\n", *(p.getSalary()));
    *pointer = 30000;
    printf("p.getSalary() is %d\n", *(p.getSalary()));
}

int main() {
    Person p("xiaoming", 11, 99);
    myCode(p);

}
