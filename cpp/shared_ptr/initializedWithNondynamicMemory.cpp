#include <cstdio>
#include <memory>
#include <iostream>

using namespace std;

class Foo {
public:
    ~Foo() {
        cout << "dectr" << endl;
    }

    void say() { cout << "I am foo" << endl; }

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
        cout << "sp.use_count() is " << sp.use_count() << ", sp2.use_count() is " << sp2.use_count() << endl; 

        /* let sp reset to built-in pointer */
        Foo *p = new Foo();
        //sp = p; //wrong, to make shared_ptr re-point to built-in pointer, use reset instead of =
        sp.reset(p);
        cout << "sp.use_count() is " << sp.use_count() << ", sp2.use_count() is " << sp2.use_count() << endl; 
        //delete p; //will lead coredump since p will also be released when sp reaching its life end

        /*let sp reset to another shared_ptr */
        //sp.reset(sp3); //can not use reset to let one shared_ptr bond to another. Use = instead
        sp = sp3;
        cout << "sp.use_count() is " << sp.use_count() << ", sp2.use_count() is " << sp2.use_count() << endl; 

        cout << "------" << endl;
    }
    return 0;
}
