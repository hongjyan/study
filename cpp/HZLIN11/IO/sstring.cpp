#include <iostream>
#include <sstream>
#include <string>
using namespace std;

int main() {
    string dump;
    istringstream iss("hello world\n");  //istringstream can use line to initialize. \
					 But cin just input one single word
    ostringstream oss;
    while (iss >> dump) {        //istringstream can write word one by one.
	cout<<dump;
	cout<<"#";

	oss<<dump;
	cout<<oss.str();
    }

    cout << endl;
    int i = 12345;
    oss << i;
    cout << oss.str() << endl;
}
