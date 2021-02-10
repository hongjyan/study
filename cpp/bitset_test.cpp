#include <iostream>
#include <bitset>
using namespace std;

int main() {
//initialize
    bitset<32> b(1U);
    cout << "b is " << b << endl;
    cout << endl << endl << endl;

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


    //initialize bitset with string
    bitset<4> bb4("1110");  //same as bb4(0b1110)
    std::cout << "bb4 is " << bb4 << std::endl;
    for (size_t i=0; i!=bb4.size(); ++i) {
        cout << bb4[i];
    }
    cout << endl;

    string str("10110100100001101");
    bitset<32> bitvec(str, 5, 4); //the 4 elements starting from str[5]. so it is 1001
    cout << "bitvec is " << bitvec << endl;
    bitset<32> bitvec2(str, str.size()-4); //use the last 4 characters
    cout << "bitvec2 is " << bitvec2 << endl;
    cout << endl << endl << endl;


//operation
    bitset<32> bitvec3(1U);
    cout << "bitvec3 is " << bitvec3 << endl;
    bool is_set = bitvec3.any();
    bool is_not_set = bitvec3.none();
    bool all_set = bitvec3.all();
    size_t sz = bitvec3.count();
    bitvec3.flip();
    cout << "bitvec3 is " << bitvec3 << endl;
    bitvec3.reset();
    cout << "bitvec3 is " << bitvec3 << endl;
    bitvec3.set();
    cout << "bitvec3 is " << bitvec3 << endl;
    bitvec3.set(1, 0);
    cout << "bitvec3 is " << bitvec3 << endl;
    bitvec3.flip(0);
    cout << "bitvec3 is " << bitvec3 << endl;
    cout << "bitvec3.test(0) is " << bitvec3.test(0) << endl;

    bitvec3[0] = 1;
    cout << "bitvec3 is " << bitvec3 << endl;
    bool bl = bitvec3[0];
    cout << "bool value is " << bl << endl;

    //to_ulong, to_ullong
    unsigned long ulong;
    unsigned long long ullong;
    bitset<128> bitvec4(1U);

    ulong = bitvec3.to_ulong();
    ullong = bitvec3.to_ullong();

    ulong = bitvec4.to_ulong();
    ullong = bitvec4.to_ullong();

    //io operation
    bitset<128> bitvec5;
    cin >> bitvec5;
    cout << "bitvec5 is " << bitvec5 << endl;
    return 0;
}
