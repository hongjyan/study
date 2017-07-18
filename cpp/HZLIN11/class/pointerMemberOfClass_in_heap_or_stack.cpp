#include <iostream>
#include <string.h>
using namespace std;
class foo {
public:
    foo(char* pd): m_data(pd) {}
    void changeData(char* str) { m_data = str;}
    void changeData(size_t pos, char newc) { *(m_data+pos) = newc;}
    void printData() { cout << m_data << endl; }
protected:
    char* m_data;
};



class bar {
public:
    bar() {
        m_data = new char[100];
        memset(m_data, 0, 100);
    }
    void changeData(char* str) { memcpy(m_data, str, strlen(str));}
    void printData() { cout << m_data << endl; }
    ~bar() {
        if (NULL != m_data) { cout << "I am deleting\n"; delete [] m_data; };
    }
protected:
    char* m_data;
};

int main(int argc, char* argv[]) {
    char p[] = {'g','a','m','e', '\0'};
    char p2[] = {'f', 'u', 'n', '\0'};
    foo f1(p);
    foo f2 = f1;
    f1.changeData(p2); //won't change f2.m_data since f1.changeData only make f1.m_data pointe to other place.
    f1.printData();
    f2.printData();
    f2 = f1;  
    f1.changeData(0,'F'); //change f2.m_data too since f1.m_data and f2.m_data point to same place.
    f1.printData();
    f2.printData();

    bar b1, b2;
    b1.changeData("game");
    b2.changeData("fun");
    b1.printData();
    b2.printData();
    b2 = b1;  //memory leak here, b2.m_data which was allocated at b2.changeData("fun") is leaking.
    b1.changeData("GAME"); //b1.m_data changed, cause b2.m_data change since they are pointing to same place.
    b1.printData();
    b2.printData();
    //b1.m_data will delete twice during destructor"
}
