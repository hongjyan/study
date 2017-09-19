#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <bitset>
#include <memory>

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
    cout << "-----------testing atoi---------------" << endl;
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
//2017/8/3
{
    std::cout << "2017/8/3------------------" << std::endl;
    struct Kludge {
        static void push(vector<int>& vec, int element) {
            vec.push_back(element);
        }
    };
    std::vector<int> ivec =  {1, 2, 3, 4};
    std::vector<int> *p = &ivec;
    std::cout << "size of *p is " << p->size() << std::endl;
    for (int& i : *p) {
        std::cout << i << std::endl;
    }
    Kludge::push(ivec, 5);
    printf("After push\n");
    for (int& i : *p) {
        std::cout << i << std::endl;
    }
    

    std::cout << "--------------" << std::endl;
    struct st_qciDscp {
        unsigned int qci;
        unsigned char dscp;
    };
    struct Kludge2 {
        static void push(vector<st_qciDscp>& vec, st_qciDscp element) {
            vec.push_back(element);
        }
    };
    auto sp_stVec = std::make_shared<std::vector<st_qciDscp> >();
    std::cout << sp_stVec->capacity() << std::endl;
    st_qciDscp qciDscp1 = {1, 5};
    sp_stVec->push_back(qciDscp1);
    sp_stVec->push_back({5, 8});
    std::cout << sp_stVec->capacity() << std::endl;
    for (st_qciDscp& qciDscp : *sp_stVec) {
        std::cout << qciDscp.qci << ":" << qciDscp.dscp << "finished" << std::endl;
        std::wcout << qciDscp.qci << ":" << qciDscp.dscp << "finished" << std::endl;
        printf("%u:%u\n", qciDscp.qci, qciDscp.dscp);
    } 
    std::cout << (*sp_stVec)[0].qci << ":" << (*sp_stVec)[0].dscp << std::endl;
    std::cout << (*sp_stVec)[1].qci << ":" << (*sp_stVec)[1].dscp << std::endl;
    Kludge2::push(*sp_stVec, {6,9});
    printf("After push\n");
    for (st_qciDscp& qciDscp : *sp_stVec) {
        std::cout << qciDscp.qci << ":" << qciDscp.dscp << "finished" << std::endl;
        std::wcout << qciDscp.qci << ":" << qciDscp.dscp << "finished" << std::endl;
        printf("%u:%u\n", qciDscp.qci, qciDscp.dscp);
    } 
}

{
    //2017.8.9
    std::cout << "------------------------2017.8.9------------------" << std::endl;
    string pre = "yhj";
    string str =  pre + " game " + " fun" + " love";
    std::cout << str << std::endl;
}

{
    //2017.8.30
    float f = 3.14;
    std::cout <<  f << std::endl;
    short s = 10;
    s = s + 1;
    std::cout << s << std::endl;
}
    return 0;
}
