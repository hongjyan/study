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
    std::cout << "std::bitset<4>(0b0111) is " << bits << std::endl;
    std::cout << "bits[0] is " << bits[0] << endl;
    std::cout << "bits[3] is " << bits[3] << endl;
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

{
    cout << "2020.3.11. sizeof on vector return the number of element than the memory occuiped by vector"  << endl;
    vector<int> vi = {1, 2, 3, 4, 5, 6, 7, 8};
    std::cout << sizeof(vi) << std::endl;
}

{
    cout << "2020.3.20, comparation between -int and unsigned int" << endl;
    unsigned int c = 274607296;
    unsigned int d = 274608256;
    unsigned int Interval = 320;
    cout << c-d << endl; //4294966336. seems "unsigned x +/- unsigned y" generate unsigned result.
    cout << (int)(c-d) << endl;//-960
    cout << Interval << endl;

    unsigned int e = 1;
    signed char f = -9;
    cout << typeid(e+f).name() << endl;
    cout << e + f << endl; 
    cout << (unsigned int)(f) << endl;
    
    cout << -3%12 << endl;
    unsigned int a = 1;
    unsigned int b = 4;
    unsigned int g = 12; 
    cout << a-b << endl;
    cout << (a-b)%g << endl; //1
}

{
    cout << "2020.4.2. vector.push_back(type &)" << endl;
    vector<string> svc;
    {
        std::string s("soemoe"); 
        svc.push_back(s);
    }
    cout << "svc.size() is  " << svc.size() << ", svc[0] is " << svc[0] << endl; //ok since vector do copy even push_back's parameter type is reference.
}
    return 0;
}
