#include <iostream>
using namespace std;

class A {
private:
    int *array;
    size_t sz;

public:
    A() { sz = 0; array = nullptr; }

    A(size_t size): sz(size) { array = new int[sz](); }

    A(const A &src): A(src.sz) {
        cout << "copy constructor" << endl;
        for(size_t i=0; i != sz; i++) {
            *(array+i) = *(src.array+i);
        }
    }

    A(A &&src) noexcept { 
       cout << "move constructor" << endl;
       sz = src.sz;
       array = src.array;
       src.array = nullptr;
       src.sz = 0; 
    }

    A& operator=(const A &rhs) { 
        cout << "operator=" << endl;
        if (this != &rhs) {
            free(); 
            sz = rhs.sz;
            array = new int[sz]();

            for (size_t i=0; i!=sz; i++) {
                *(array+i) = *(rhs.array+i);
            }
        }
        return *this;
    }

   
    A& operator=(A &&rhs) noexcept {
        cout << "move operator=" << endl;

        if (this != &rhs) {
            delete [] array;   
            sz = rhs.sz;
            array = rhs.array;
            rhs.sz = 0;
            rhs.array = nullptr;
        }
        return *this;
    }
    
    int& operator[](int i) {
        return *(array+i);
    }

    int& operator[](int i) const {
        return *(array+i);
    }
 
    void free() {
        delete [] array;
    }

    ~A() {
        cout << "de-constructor" << endl;
        free();
        sz = 0;
        array = nullptr;
   }
   
   const size_t getSize() const { return sz; }
   int *getArray() { return array; }
};

void print(const A &aw) {
     for (size_t i=0; i!=aw.getSize(); i++) {
         cout << aw[i] << " ";
     }
     cout << endl;
}

A foo() {
    A aw(20);
    return aw;
}

A&& bar() {
    A aw(30);
    return std::move(aw);
}

int main() {
   A a;
   A a2(10);
   a2[0] = 0;
   a2[1] = 1;
   cout << "a is ";
   print(a);
   cout << "a2 is ";
   print(a2);
   
   A a3(a2); //copy-ctr
   cout << "a3 is ";
   print(a3);
   A a4(std::move(a3)); //move-constructor
   cout << "after be moved, a3 is " ; 
   print(a3);
   cout << "after move-ctr, a4 is ";
   print(a4);
   
   a2[0] = 10;
   a2[1] = 11;
   a3 = a2;  //operator=
   cout << "after assigment, a3 is ";
   print(a3);
   cout << "a4 still be ";
   print(a4);  //aw4.array is there still since aw3.array set as nullptr during move-ctr.

   cout << "A a5 = foo()" << endl;
   A a5 = foo(); //copy elide 
 
   cout << "a5 = bar();" << endl;
   a5 = bar(); //move operator?
   cout << "A a6 = bar();" << endl;
   A a6 = bar(); //move ctr   
   //A &&a7 = a6; //complie error?
   cout << "A &&a8 = A()" << endl;
   A &&a8 = A(); //bond or move-ctr? A: bond
   cout << "A a9 = a8;" << endl;
   A a9 = a8; //copy-ctr, since a8 has a name even a8 is lvalue reference

   return 0;
}   
