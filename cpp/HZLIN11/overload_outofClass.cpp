#include <iostream>
using namespace std;

void Output(int x);
void Ouput(int x, float y=3.14);
void Output(int x) {
    cout<<"x is "<<x<<endl;
}

void Output(int x, float y) {
    cout<<"x is "<<x<<endl;
    cout<<"y is "<<y<<endl;
}

int main() {
    int x=1;
    float y=0.5;
    Output(x, y);
    Output(x); //suprise, no error!----ambiguous call
}
