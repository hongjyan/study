#include <iostream>
#define STRINGIFY2(x) #x
#define STRINGIFY(x) STRINGIFY2(x)
#define FUNC_LINE2(a, b) a##b
#define FUNC_LINE(a, b) FUNC_LINE2(a, b)
#define SOME_MACRO __func__ //__func__ is not a macro than pre-identifier. it won't be expanded other than __LINE__ during pre-process.



using namespace std;

int main() {
    //cout <<  FUNC_LINE2(SOME_MACRO, __LINE__) << endl; //'SOME_MACRO__LINE__' was not declared in this scope
    //cout << FUNC_LINE(SOME_MACRO, __LINE__) << endl; //error: 'function_name14' was not declared in this scope
    cout << STRINGIFY2(FUNC_LINE2(SOME_MACRO, __LINE__)) << endl;
    cout << STRINGIFY2(FUNC_LINE(SOME_MACRO, __LINE__)) << endl;
    cout << STRINGIFY(FUNC_LINE2(SOME_MACRO, __LINE__)) << endl;
    cout << STRINGIFY(FUNC_LINE(SOME_MACRO, __LINE__)) << endl;
    return 0;
}



