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

    const int TEST{12};
    int arr_[TEST]; //error: invalid use of non-static data member 'A::TEST'. Theoretically, it should work I assume.

    //way1: 
    static const size_t s_size = 100;  //for whole class
    int arr1[s_size];
    //way2:
    enum { SIZE1 = 100, SIZE2 = 200 };  //for single instance.
    int arr2[SIZE1];

};

A::A(int size): c_size(size) {};

void A::output() {
    cout<<"c_size is "<< c_size <<endl;
    cout<<"size is "<< size <<endl;
    cout<<"size of arr1 is "<<sizeof(arr1)<<endl;
    cout<<"size of arr2 is "<<sizeof(arr2)<<endl;
    cout << "const int is " << TEST << endl;
}


int main() {
    A a(22);
    a.output();

    constexpr int SIZE6 = 600;
    int arr6[SIZE6];
}
    
