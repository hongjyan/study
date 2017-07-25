#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <bitset>

using namespace std;

//2017.4.6
class Foo {
public:
    Foo(const std::string& name): name_(name) {}
    //void setName(const std::string& name) { name_ = name; } //wrong, can not change const reference content
private:
    const std::string& name_;
};

int main() {
    vector<int> ivec;
    std::string bar("foo");
//2017.3.20, will overflow to assign big value
    //char c = 12222;  //overflow
    //int i = 12222;
    //c = i; // not overflow

//2017.3.22 
    cout << sizeof(long) << endl;
//2017.3.28
    unsigned int j = atoi("game");
    cout << j << endl;
    j = atoi("-1");
    cout << j << endl;
//2017.4.6
//    Foo foo("game");
//    foo =  Foo("fun");

//2017.5.18
{
    const char* s = "game";
    printf("%s\n", s+1);
}

//2017.6.5 
{
    std::string name = "";
    printf("%d\n", name.empty());
}
{
    std::string line;
    std::getline(std::cin, line);
    printf("lins is %s\n", line.c_str());
}
//2017.7.6
{
    typedef struct messageHeader {
        short magicCode;
        short length;
        unsigned char content[0];
    }st_msgHeader;
    
    st_msgHeader msgheader = {123, 30};
    printf("sizeof(st_msgHeader) is %lu, magicCode is %hd, length is %hd\n", 
		    sizeof(st_msgHeader), msgheader.magicCode, msgheader.length);
    unsigned char buffer[8] = {1, 2, 3, 4, '5', '6', '7', '\0'};
    st_msgHeader *pmsgHeader = (st_msgHeader*)buffer;
    printf("msg content is %s\n", pmsgHeader->content);
}
//2017/7/25
{
    std::bitset<4> bits = 0b0111;
    std::cout << bits << std::endl;
    bits.reset(0);
    std::cout << bits << std::endl;

}
    return 0;
}
