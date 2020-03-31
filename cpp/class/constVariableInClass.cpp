#include <iostream>
using namespace std;

class A {
public:
    A(int);
    void output();
private:
    const int c_size;   //const variable for object.
    int size = 300; //available only with -std=c++11
    // int arr_[size]; //Error! array's length can change if this line is OK.

    //const int TEST = 12;  //ISO C++ forbids initialization of member.
    //way1: 
    static const size_t s_size = 100;
    int arr1[s_size];
    //way2:
    enum { SIZE1 = 100, SIZE2 = 200 };  //use enum type to realize const variable for class.
    int arr2[SIZE1];

    constexpr int SIZE5 = 500;
    int arr5[SIZE5];


};

A::A(int size): c_size(size) {};

void A::output() {
    cout<<"c_size is "<< c_size <<endl;
    cout<<"size is "<< size <<endl;
    cout<<"size of arr1 is "<<sizeof(arr1)<<endl;
    cout<<"size of arr2 is "<<sizeof(arr2)<<endl;
}


int main() {
    A a(22);
    a.output();
}
    
