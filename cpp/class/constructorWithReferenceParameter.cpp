#include <cstdio>
#include <string>

class Bar {
public:
    Bar() {
	printf("constructing Bar\n");
    }
   
    Bar(Bar& src) {
	printf("copy-constructor of Bar\n");
    }

    Bar& operator=(const Bar& src) {
	printf("assign-operator of Bar\n");
	num = src.num;
	return *this;
    }
    
    int getNum() {
	return num;
    }

    int setNum(int n) {
	num = n;
	return num;
    }

    ~Bar() {
        num = 123; 
        printf("De-constructing bar at address %p\n", this);
    }
private:
    int num;
};


class Foo {
public:
    //TO summary: parameter's copy-constructor  will be call when parameter is object, while won't when parameter is reference

    Foo(Bar b): bar(b), r_bar(b) {
        printf("address of argument is %p\n", &b); 
        printf("constructing foo(Bar)\n");  //r_bar bond to argument:b which is a temporary object who will be destroied when it is out of this function's range. danger undefined behavior.
    }

    Foo(int i, Bar& b): bar(b), r_bar(b) { printf("constructing foo(int, Bar&)\n"); }
 
    Bar& getReferenceBar() {
	return r_bar;
    }

    Bar getBar() {
	return bar;
    }

private:
    Bar bar;
    Bar& r_bar;
};


class Hei {
public:
    Hei() { printf("constructor of Hei\n"); }
    Hei(const Hei& h) {
	printf("copy-constructor of Hei\n");
    }
    Hei& operator=(const Hei& src) {
	printf("operator= of Hei\n");
	return *this;
    }
    
};
    
Bar &getBar(Bar b) {
    Bar &rb = b;
    return rb;
}

int main() {
    Bar b;
    b.setNum(11);
    printf("b.num is %d\n", b.getNum());
    printf("---------------------------\n");

    Foo f(b);
    printf("f.getReferenceBar().getNum is %d\n", f.getReferenceBar().getNum()); //123. I thought it will be an undefined value for example -123124134132 since argument of function is temporary which will be destroied.  undefined danger behavior
    printf("the address of reference bar is %p\n", &(f.getReferenceBar()));
    Bar &b2 = getBar(b);
    printf("the address of gotBar(b) is %p\n", &b2);
    printf("---------------------------\n");

    Foo f1(1, b);
    b.setNum(12);
    printf("f1::b::num is %d\n", f1.getReferenceBar().getNum()); //12
    printf("---------------------------\n");

    Hei hei;
    Hei h = hei; //copy constructor even there is =
    return 0;
}
