#include <iostream>
#include <vector>
using namespace std;

int main() {
    char str[] = "this is a test";
    vector<char> c_vec(str, str + sizeof(str)/sizeof(char) );

//value_type
//    cout<<"the element's type of c_vec is "<<c_vec.value_type<<endl; //seems can't use object.value_type.\
	Only vector<char>::value_type is legal, then what's meanning?
}
