#include <cstdio>
#include <memory>
#include <iostream>

using namespace std;

class Foo {
public:
    ~Foo() {
        cout << "dectr" << endl;
    }

};

void customer_delete(const Foo *pFoo) {
    cout << "do something on " << pFoo << endl;
}

int main() {
    Foo obj1;
    {
        cout << "------" << endl;
        //shared_ptr<Foo> p(&obj1); //COREDUMP. &obj1 will be delete once more without customer-delete
        shared_ptr<Foo> p1(&obj1, \
                          [] (const Foo *pFoo) { cout << "do something on " << pFoo << endl;});
        shared_ptr<Foo> p2(&obj1, customer_delete); //not good practice, but run ok since customer_delete do none of memory
        p2.reset();  //ok practice.
        //p2.reset(&obj1);  //COREDUMP. &obj1 will be delete once more without customer-delete
        p2.reset(&obj1, customer_delete); //not good practice but run ok since customer_delete do none of memory
        cout << "------" << endl;
    }
    return 0;
}
