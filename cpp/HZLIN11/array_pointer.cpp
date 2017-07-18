#include <iostream>
#include <cstdlib>
#include <string.h>
using namespace std;
int main() {
    char str[] = "hello";
    char *p = str;
    char *mp = (char *) malloc(100);
    strcpy(mp, "tomato, potato");
    cout<<"str is "<<str<<endl;
    cout<<"p is "<<p<<endl;
    cout<<"mp is "<<mp<<endl;
    cout<<"size of str is "<<sizeof(str)<<endl; //str is array here.
    cout<<"size of p is "<<sizeof(p)<<endl;  //p is pointer here.
    cout<<"size of mp is "<<sizeof(mp)<<endl; //mp is a pointer still though it points to bigger memory.
}
