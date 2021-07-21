#include <iostream>
#define STRINGIFY2(x) #x
#define STRINGIFY(x) STRINGIFY2(x)
#define FUNC_LINE2(a, b) a##b
#define FUNC_LINE(a, b) FUNC_LINE2(a, b)
#define SOME_MACRO __func__ //__func__ is not a macro than pre-identifier. it won't be expanded other than __LINE__ during pre-process.


#define PLUS    +
#define EQUAL      =
#define PLUS_EQU(p, e) pe
#define PLUS_EQU2(p, e) p##e
#define PLUS_EQU3(p, e) PLUS_EQU2(p, e)



#define AFTERX(x) X_##x
#define XAFTERX(x) AFTERX(x)
#define TABLESIZE2 TABLESIZE
#define TABLESIZE 1024
#define BUFSIZE TABLESIZE


using namespace std;

int main() {
    //cout <<  FUNC_LINE2(SOME_MACRO, __LINE__) << endl; //'SOME_MACRO__LINE__' was not declared in this scope
    //cout << FUNC_LINE(SOME_MACRO, __LINE__) << endl; //error: ‘__func__14’ was not declared in this scope
    cout << STRINGIFY2(FUNC_LINE2(SOME_MACRO, __LINE__)) << endl;
    cout << STRINGIFY2(FUNC_LINE(SOME_MACRO, __LINE__)) << endl;
    cout << STRINGIFY(FUNC_LINE2(SOME_MACRO, __LINE__)) << endl;
    cout << STRINGIFY(FUNC_LINE(SOME_MACRO, __LINE__)) << endl;

    cout << STRINGIFY(PLUS_EQU(PLUS, EQUAL)) << endl;
    cout << STRINGIFY(PLUS_EQU2(PLUS, EQUAL)) << endl;
    cout << STRINGIFY(PLUS_EQU3(PLUS, EQUAL)) << endl;


    cout << STRINGIFY(AFTERX(BUFSIZE)) << endl;
    cout << STRINGIFY(XAFTERX(BUFSIZE)) << endl;
    return 0;
    
}



