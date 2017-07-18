#include <iostream>
#include <fstream>
using namespace std;

int main() {
    ostream *p_prevstr;
    ofstream filestr;
    filestr.open("test.txt");

    *cin.tie()<<"this is inserted into cout\n";
    p_prevstr = cin.tie(&filestr);
    *cin.tie()<<"this is inserted into file\n";
    cin.tie(p_prevstr);

    filestr.close();
}
