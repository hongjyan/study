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
        if (array) {  //delete nullptr will crash
          delete [] array;
        }
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


//best practice. will prefer move-ctr(rvalue assign to rreference) than cpy-ctr(rvalue to reference).
A foo() {
    A aw(20);
    aw[0] = 3;
    aw[1] = 4;
    return aw;  
}


/* bad!!! return a reference to temporary object
A& foo2() {
    A aw(20);
    aw[0] = 33;
    aw[1] = 44;
    return aw;  
}
*/


/*error: cannot bind 'A' lvalue to 'A&&'
A&& foo2() {
    A aw(20);
    aw[0] = 33;
    aw[1] = 44;
    return aw;  
}
*/

//work too, but std::move on aw is unncessary. 
A bar() {
    A aw(30);
    aw[0] = 33;
    aw[1] = 44;
    return std::move(aw); //std::move cast lvaule:aw to rvalue reference. And return type is A, so will call move-ctr during return to create a temporary rvalue 
}

int main() {
   //copy elide, no ctr
   A a = foo();
   print(a);
   const A& lref_a =  foo(); //const reference extend temporary object lifecycle.
   print(lref_a);
   A&& rref_a = foo();  //same as const A&, rvalue ref extend temporary object lifecycle.
   print(rref_a);  //it is ok to assign a rrefrence to reference
   cout << endl << endl << endl;



   a = foo(); //move-opeator.
   A a2(std::move(a)); //move-ctr. std::move cast lvalue to rvalue referene, so will use move-ctr
   cout << "after be moved, a is " ; 
   print(a);
   cout << "after move-ctr, a5 is ";
   print(a2);
   cout << endl << endl << endl;



   a2 = bar(); //move-operator.
   print(a2);
   A a3 = bar(); //copy elide
   print(a3);
   cout << endl << endl << endl;



   //A &&a4 = a3; //complie error. Not allow assign left value to right reference.
   A &&a4 = A(); //legal to bond rvalue to rvalue reference.
   A a5 = a4; //copy-ctr rather than move-ctr, since a8 has a name even a8 is lvalue reference
   cout << endl << endl << endl;


   return 0;
}   
