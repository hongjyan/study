#include <stdio.h>
#include <string>

class Person {
public:
    Person(const std::string &name, const int age) : name(name), age(age) 
    {
        printf("constructing Person\n");
    };

    Person(const Person& p) : name(p.name), age(p.age) {
        printf("copy constructing Person\n");
    } 

    std::string name;
    int age;
};

   
struct  st_classMember {
    Person p;
};

struct  st_primeMember {
   int number;
};


const struct st_classMember&  f1(const struct st_classMember &m) {
    printf("I am f1\n");
    return m;
}

void f2(const struct st_primeMember &m) {
    printf("I am f2\n");
}


void f3(const Person &p) {
    printf("I am f3\n");
}

int main()
{
    Person p("xiaoming", 12);
    struct st_classMember cm = {p};
    struct st_primeMember pm = {10};
    printf("************************************\n");
    f1(cm);
    printf("************************************\n");
    printf("------------------------------------\n");
    struct st_classMember cm2 = f1(cm);
    printf("------------------------------------\n");
    f2(pm);
    f3(p);
    return 0;
}
