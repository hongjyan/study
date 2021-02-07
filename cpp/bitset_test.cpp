#include <iostream>
#include <bitset>
using namespace std;

int main() {
    //cout is in reverse as index order.
    std::bitset<4> bits = 0b1110;
    std::cout << "bits[0] is " << bits[0] << endl; //least signicant bit at lowest index
    std::cout << "bits[3] is " << bits[3] << endl;  //most signicant bit ar highest index
    std::cout << "std::bitset<4>(0b1110) is " << bits << std::endl;
    for (size_t i=0; i!=bits.size(); ++i) {
        cout << bits[i];
    }
    cout << endl << endl << endl;


    //pading with 0
    std::bitset<17> b17(0xffff);
    std::cout << "b17 is " << b17 << std::endl;
    for (size_t i=0; i!=b17.size(); ++i) {
        cout << b17[i];   
    }
    cout << endl << endl << endl;
   
    //truncate high bits
    std::bitset<12> b12(0xf001);
    std::cout << "b12 is " << b12 << std::endl;
    for (size_t i=0; i!=b12.size(); ++i) {
        cout << b12[i];   
    }
    cout << endl << endl << endl;
    return 0;
}
