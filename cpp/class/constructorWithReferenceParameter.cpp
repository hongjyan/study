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
        printf("De-constructing bar\n");
    }
private:
    int num;
};


class Foo {
public:
    //To summary, 
    //Take object:Bar as copy-constructor parameter, then copy-constructing of bar will be called.
    //Take reference:Bar as copy-constructor parameter, then copy-constructing of bar won't be called.
    //if Foo::bar is object, then bar(b) will call Bar's copy-constructor.
    //if Foo::bar is reference, then bar(b) won't call Bar's copy-constructor.
    Foo(Bar b): bar(b), r_bar(b) { printf("constructing foo(Bar)\n"); } //Foo(Bar b) will cause bar's copy-constructor, bar(b) will cause bar's copy-constructor. r_bar(b) won't cause bar's copy-constructor.
    Foo(int i, Bar& b): bar(b), r_bar(b) { printf("constructing foo(int, Bar&)\n"); }
 
    void fun(Bar& b) { printf("fun(Bar& b)\n"); }

    void havefun(Bar b) { printf("havefun(Bar b)\n"); }

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
    const Hei& getHei() {
	return *this;
    }
    
};
    

int main() {
    Bar b;
    b.setNum(11);
    printf("b.num is %d\n", b.getNum());
    printf("---------------------------\n");
    Foo f(b);
    printf("f.getReferenceBar().getNum is %d\n", f.getReferenceBar().getNum()); //will be -1568605280 since temporary Bar as constructor's parameter will be destoryed.
    b.setNum(12);
    printf("f.getReferenceBar().getNum is %d after set\n", f.getReferenceBar().getNum());
    Bar& rb = f.getReferenceBar();
    printf("rb.getNum() is %d\n", rb.getNum());
    rb.setNum(12);
    printf("rb.getNum() is %d\n", rb.getNum());
    printf("---------------------------\n");
    Foo f1(1, b);
    printf("f1::b::num is %d\n", f1.getReferenceBar().getNum());
    printf("---------------------------\n");
    f.fun(b);
    printf("---------------------------\n");
    f.havefun(b);
    printf("---------------------------\n");
    b.setNum(13);
    printf("f1::b::num is %d after change\n", f1.getReferenceBar().getNum());
    printf("---------------------------\n");
    Bar b1 = f1.getReferenceBar();
    printf("---------------------------\n");
    std::string s1 = "game";
    std::string s2 = s1;
    //Bar b2 = f1.getBar();
    printf("---------------------------\n");
    Hei hei;
    Hei h = hei;
    Hei h1 = hei.getHei();
    return 0;
}
