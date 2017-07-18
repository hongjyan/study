#include <cstdio>
#include <string.h>


class Foo {
public:
    int setAge(int age) {
        _age = age;
        return _age;
    }

    int getAge() {
        return _age;
    }

private:
    int _age;
};

Foo getFoo() {
    Foo f;
    f.setAge(9);
    return f;
}

class Bar {
public:
    Bar() {
        _name = new char[100];
        printf("constructing bar\n");
    }
    
    Bar(const Bar& src) {
        printf("copy-constructor bar\n");
        this->_name = src._name;
    }

    const char* setName(const char* name) {
        memcpy(_name, name, strlen(name));
        return _name;
    }

    const char* getName() {
        return _name;
    }
    
    ~Bar() {
        if (_name) delete _name;
        printf("de-constructing bar\n");
    }

private:
    char* _name;
};

Bar getBar() {
    printf("Enter Bar\n");
    Bar b;
    b.setName("xiaoming");
    printf("Exit bar\n");
    return b;
}

int main()
{
    Foo f1 = getFoo();
    Bar b1 = getBar(); //the bar constructing in getBar() won't be released in this line. It is compiler-optimum i think.
                       //seems b1 is align of bar in getBar().
    Bar b2 = b1;     

    printf("f1.age is %d, b1.name is %s\n", f1.getAge(), b1.getName());
    printf("f1.age is %d, b1.name is %s, b2.name is %s\n", f1.getAge(), b1.getName(), b2.getName());
    return 0;
}


