#include <iostream>
#include <string>
using namespace std;

class Test {
public:
    Test(string na, string se): name(na), sex(se) {}
    
    void output(const Test &T) {
	cout<<"my name is "<<name<<endl;
	cout<<"my neighbour's name is "<<T.name<<endl;
	cout << "my neighbour's sex is " << T.sex << endl;
    }
    
    Test& operator=(const Test &T) {
        name = T.name;
        sex = T.sex;
        return *this;
    } 
	
protected:
    string name;

private:
    string sex;
};



int main() {
    Test T1("Li Lei", "male");
    Test T2("Han Meimei", "female");
    T1.output(T2);
//verify accessiable also in assign constructor;
    T2 = T1; 
    T2.output(T1);
}
