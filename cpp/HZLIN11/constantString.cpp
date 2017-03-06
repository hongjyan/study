#include <iostream>
using namespace std;
int main() {
//declare a pointer and point it to constant-characters, then can't change the memory where stores the constant characters via this pointer.
    char *p = "game";
//    *p = 'T';        //a segmentation fault which won't be detected during compiling.


//can let pointer:p to point another address. 
    p = "fun";       //right, p will be re-pointed to another address which was constantly initialized by "fun".
    cout<<*p<<endl;  //*p is the first element of this address.
    cout<<p<<endl;

//declare a pointer, set its value to NULL. 
    char *p2 = NULL;
//    *p2 = 'T';        //can't initial, since a NULL pointer doesn't point anywhere. 
    cout<<p2<<endl;


//declaring an array will applying memory for this array, to store something.
    char a[] = "game";
    cout<<a<<endl;
//    a = "funi";  //can't re-assign array. Notice: can re-assign vector.
//    a[1] = "T";  //"T" is an string, not charactor.
    a[1] = 'T';
    cout<<a<<endl;
    return 0;
}
