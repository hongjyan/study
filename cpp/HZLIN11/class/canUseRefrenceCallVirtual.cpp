#include <stdio.h>
#include <stdlib.h>

class Base {
public:
    void virtual foo() {
	printf("foo in base\n");
    }
};

class Derived : public Base {
public:
    void virtual foo() {
	printf("foo in derived\n");
    }
};

void callClass(Base &b) {
   b.foo();
}

int main() {
    Derived d;
    callClass(d);
    //Base b;
    //callClass(b);
    return 0;
}
    
