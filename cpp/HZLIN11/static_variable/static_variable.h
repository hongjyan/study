#include <iostream>
using namespace std;

#ifndef STATIC
#define STATIC
void f1() {
    static bool flag = true;
    cout << "flag is " << flag << endl;
    flag = false;
}

//if static local variable effect among file
//void call_f1() {
//    cout << "flag is " <<  flag << " in call_f1()" << endl;
//}
#endif
    
