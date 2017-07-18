#include <iostream>
#include <string.h>
using namespace std;

int main() {
    int *p = new int(1);
    char *str = new char[10];
    strcpy(str, "funny");
    cout<<*p<<endl;
    cout<<p<<endl;
    cout<<str<<endl;
    delete [] p;  //can use delete [] for single memory
    delete [] str;
}
