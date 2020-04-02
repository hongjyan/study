#include <cstdio>
#include <string>
#include <iostream>

using namespace std;

class Foo {
public: 
    int getX() { return x_; }
    string getStr() { return s_; }
private:
    int x_;
    string s_;
}; 


class Bar {
public: 
    Bar(int x, string s): x_(x), s_(s) {}
    int getX() { return x_; }
    string getStr() { return s_; }
private:
    int x_;
    string s_;
}; 

int main() {
//initialization in global scope
    int i; //ok but bad practice using default initialize.
    //int i2() //wrong. i2 taken as function name
    int i2(1);   
    //int i3{} //wrong. i3 taken as int (*) ()
    int i3{1};
    int *p = new int; //0. ok but bad practice using default-initialize
    int *p2 = new int(); //0. ok && value-initialize
    int *p3 = new int{}; //0. ok. value-intialize?
    printf("i is %d, i2 is %d, i3 is %d, *p is %d, *p2 is %d, *p3 is %d.\n", i, i2, i3, *p, *p2, *p3); 
    int k{10.1}; //it is told wrong at C++ prime, but just warnning here.
    printf("k is %d\n", k);    

    Foo foo; 
    Foo foo2(); //Wrong but compile OK. will lead succeeding error using foo2.
    Foo foo3{}; //class list initialize
    //Bar bar{}; //error
    Foo *pf = new Foo; //ok but bad practive here using default intialize.
    Foo *pf2 = new Foo();//ok. value-intialize
    Foo *pf3 = new Foo{}; //ok. seems call default ctr
    //Bar *pb = new Bar{}; //Error
    printf("foo.x = %d, foo.s = %s\n", foo.getX(), foo.getStr().c_str());
    //printf("foo2.x = %d, foo2.s = %s\n", foo2.getX(), foo2.getStr().c_str());
    printf("foo3.x = %d, foo3.s = %s\n", foo3.getX(), foo3.getStr().c_str());
    printf("pf->getX = %d, pf->getStr = %s\n", pf->getX(), pf->getStr().c_str());
    printf("pf2->getX = %d, pf2->getStr = %s\n", pf2->getX(), pf2->getStr().c_str());
    printf("pf3->getX = %d, pf3->getStr = %s\n", pf3->getX(), pf3->getStr().c_str());


    //test initialize in non-global scope
    {
        printf("-----------------------------non-global scope-----------------------\n");
 
        int i;
        int *p = new int;
        int *p2 = new int();
        int *p3 = new int{};
        printf("i is %d, *p is %d, *p2 is %d, *p3 is %d\n", i, *p, *p2, *p3);

        Foo foo; 
        Foo foo3{}; //class list initialize
        Foo *pf = new Foo; //ok but bad practive here using default intialize.
        Foo *pf2 = new Foo(); //ok. value-intialize
        Foo *pf3 = new Foo{}; //ok. but why?
        printf("foo.x = %d, foo.s = %s\n", foo.getX(), foo.getStr().c_str()); //0, empty
        printf("foo3.x = %d, foo3.s = %s\n", foo3.getX(), foo3.getStr().c_str());
        printf("pf->getX = %d, pf->getStr = %s\n", pf->getX(), pf->getStr().c_str());
        printf("pf2->getX = %d, pf2->getStr = %s\n", pf2->getX(), pf2->getStr().c_str());
        printf("pf3->getX = %d, pf3->getStr = %s\n", pf3->getX(), pf3->getStr().c_str());  
    }

    {
        printf("---------------------test auto & initialize--------------------------\n");
        auto p = new string("fun");
        auto p1 = new auto(string("fun"));
        auto p2 = new auto("fun"); //ok. unbelievable!
        //auto p3 = new auto(10, 'c'); //error
        auto p3 = new string(10, 'c');

        //new array
        auto q = new int[10]; //bad practice with unintialization
        auto q2 = new int[10](); //good with value initialization by 0 
        auto q3 = new string[10]{"fun", "happy"};
        //auto q4 = new auto[10]{"fun", "happy"}; //wrong.
        cout << "*p is " << *p << endl;
        cout << "*p1 is " << *p1 << endl;
        cout << "*p2 is " << *p2 << endl;
        cout << "*p3 is " << *p3 << endl;
   } 
           
    return 0;
}
