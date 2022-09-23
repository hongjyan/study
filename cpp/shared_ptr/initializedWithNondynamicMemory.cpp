#include <cstdio>
#include <memory>
#include <iostream>

using namespace std;

struct IFoo {
    virtual ~IFoo() = 0;
    virtual void say() = 0;
};
IFoo::~IFoo() {}

class Foo : public IFoo {
public:
    ~Foo() {
        cout << "dectr" << endl;
    }

    void say() { cout << "I am foo" << endl; }
};

struct AnotherFoo : public IFoo {
    ~AnotherFoo() {
        cout << "AnotherFoo dectr" << endl;
    }

    void say() { cout << "I am AnotherFoo" << endl; }
};

using Func = void(*)();
void say1() {
    cout << "hey" << endl;
}

void say2() {
    cout << "hello" << endl;
}

struct FooDecorator : public IFoo {
    FooDecorator(const shared_ptr<IFoo>& ifoo, const Func& func) : ifoo_(ifoo), func_(func) {}
    void say() {
        ifoo_->say();
        func_();
    }
    shared_ptr<IFoo> ifoo_;
    Func func_;
};

void customer_delete(const Foo *pFoo) {
    cout << "do something on " << pFoo << endl;
}

int main() {
    Foo obj1;
    {
        cout << "------" << endl;
        shared_ptr<Foo> sp(&obj1, customer_delete); 
        sp.reset();  //sp reset to null, when try delete obj1, call customer_delete instead of call obj1.~() and release obj1's memory. So it is still OK call obj1.say at line below.
        obj1.say();

        /* let sp and sp2 share same memory */
        shared_ptr<Foo> sp2 = make_shared<Foo>(); 
        sp = sp2;
        shared_ptr<Foo> sp3(sp2);
        cout << "sp.use_count() is " << sp.use_count()
            << ", sp2.use_count() is " << sp2.use_count() 
            << ", sp3.use_count() is " << sp3.use_count() << endl; 

        /* let sp reset to built-in pointer */
        Foo *p = new Foo();
        //sp = p; //wrong, to make shared_ptr re-point to built-in pointer, use reset instead of =
        sp.reset(p);
        cout << "sp.use_count() is " << sp.use_count()
            << ", sp2.use_count() is " << sp2.use_count() 
            << ", sp3.use_count() is " << sp3.use_count() << endl; 
        //delete p; //will lead coredump since p will also be released when sp reaching its life end

        /*let sp reset to another shared_ptr */
        //sp.reset(sp3); //can not use reset to let one shared_ptr bond to another. Use = instead
        sp = sp3;
        cout << "sp.use_count() is " << sp.use_count()
            << ", sp2.use_count() is " << sp2.use_count() 
            << ", sp3.use_count() is " << sp3.use_count() << endl;

        cout << "2022.09.21------" << endl;
        shared_ptr<IFoo> foo = make_shared<Foo>();
        shared_ptr<IFoo> fooCopy = foo;
        fooCopy->say();
        foo = make_shared<AnotherFoo>(); //foo原来指向内存引用计数-1， 但没有释放， 所以下面一行依然有效
        fooCopy->say();

        cout << "2022.09.21.17------居然可以使用指向自己的share_ptr构造新的自己。\
        难道老自己内存在赋值时会保留而非释放" << endl;
        shared_ptr<IFoo> fooDecorator = make_shared<FooDecorator>(make_shared<Foo>(), say1);
        fooDecorator->say();
        cout << "111111111" << endl;
        fooDecorator = make_shared<FooDecorator>(fooDecorator, say2);
        fooDecorator->say();
        cout << "2222222222" << endl;

        cout << "------" << endl;
    }
    return 0;
}