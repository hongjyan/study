#include <iostream>
using namespace std;

class Obj {
public:
    Obj(void) { cout<<"construct without parameter"<<endl; }
    Obj(int x) { cout<<"construct with parameter "<<x<<endl; }
    ~Obj() { cout<<"destruct"<<endl; }
};

void Test(void) {
    Obj *p = new Obj;
    Obj *p1 = new Obj(2);
    delete  p;
    delete p1;
    
}

void Test1(void) {
    Obj *p = new Obj[10];
    //Obj *p1 = new Obj[10](2); //error: ISO C++ forbids initialization in array new
    Obj *p2 = new Obj[10];
    //delete p; //wrong!, will just delete the first object, will cause core dumped.
    delete [] p;
    delete [] p2;
}

int main(void) {
    Test();
    Test1();
}
    
