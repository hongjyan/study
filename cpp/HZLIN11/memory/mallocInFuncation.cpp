#include <iostream>
#include <cstdlib>
#include <cstring>
#include <typeinfo>
using namespace std;

//malloc a memory block, can change this memory's content by the pointer later.
char *getMemory(int num) {
    char *p = (char *) malloc(sizeof(char) * num);
    strcpy(p, "gameya");
    return p;
}

void output_p() {
    char *str = NULL;
    str = getMemory(98);
    cout<<"str is "<<str<<endl;
    strcpy(str, "hi,fun");
    cout<<"str after change is "<<str<<endl;
    free(str);
    str = NULL;
}

/* The compile info of function 
 * mallocInFuncation.cpp: In function ‘char* getMemory1()’:
 * mallocInFuncation.cpp:30: warning: address of local variable ‘a’ returned
 */
char *getMemory1() {
    char a[100] = "a beautiful girl";
    return a;
}


void output_p1() {
    char *str = NULL;
    str = getMemory1();
    cout<<"type of str is "<<typeid(str).name()<<endl;
    cout<<"str is "<<str<<endl;
    strcpy(str, "hi,fun");
    cout<<"str after change is "<<str<<endl;
}
/* The compile info of function 
 * mallocInFuncation.cpp: In function ‘char* getMemory2()’:
 * mallocInFuncation.cpp:47: warning: deprecated conversion from string constant to ‘char*’
 */
char* getMemory2() {
    char *p = "hello world";  //constant data is stored in static memory which won't be \
				released out of scope
    return p;
}

void output_p2() {
    char *str = NULL;
    str = getMemory2();
    cout<<"str is "<<str<<endl;
}

int main() {
    output_p();
    output_p1();
    output_p2();
}
