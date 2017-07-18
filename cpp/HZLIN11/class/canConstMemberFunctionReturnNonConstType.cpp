#include <stdio.h>
#include <string>
//In a const-function, every data member becomes const in such way that it cannot be modified.

class Person {
public:
    Person(std::string name, int age, int score): name(name), age(age), score(score) { salary = new int(0); }

    const std::string& name_good() const {  // Right: the caller can't change the Person's name
        return name;
    }

//Wrong, the caller can change the Person's name.
//Compile point of view, it is NOK for std::string& X = const std::string name.
/*
    std::string& name_evil() const {
        return name;
    }
*/

//Right. The caller can not change the Person's age.
//Compile point of view, it is OK for int x = const int age.
    int get_age() const {                   
        return age;
    }

//Wrong, the caller can change the Person's score.
//Compile point of view, NOK for int &x  = const int age;
/*
    int & get_score() const {
        return score;
    }
*/

//seems wrong, since the caller can change the Person's salary. But it is right since const here means caller can not change the value of returned pointer, but not the value to which the pointer points though it is not what we expected. 
//Compile OK. since "int *salary" become "int * const salary" but not "const int *salary". It is OK for int *x = int * const salary
    int* get_salary() const {
        return salary;
    }

    ~Person() {
        delete salary;
    }
private:
    std::string name;
    int age;
    int score;
    int *salary;
};


void myCode(const Person& p)  // myCode() promises not to change the Person object...
{
//    p.name_evil() = "Igor";     // But myCode() changed it anyway!!
//    printf("p.name_evil() is %s\n", p.name_evil().c_str());
   
    int *pointer = p.get_salary();
    printf("p.get_salary() is %d\n", *(p.get_salary()));
    *pointer = 30000;
    printf("p.get_salary() is %d\n", *(p.get_salary()));
    pointer = new int(12); //ok here, since asigning pointer by another pointer is value-transmit 
    printf("p.get_salary() is %d\n", *(p.get_salary()));
}

int main() {
    Person p("xiaoming", 11, 99);
    myCode(p);

}
