#include <iostream>
#include <cstdint>
using namespace std;
union S
{
    std::int32_t n;     // occupies 4 bytes
    std::uint16_t s[2]; // occupies 4 bytes
    std::uint8_t c;     // occupies 1 byte
};                      // the whole union occupies 4 bytes
 

int main()
{
    S s = {0x12345678}; // initializes the first member, s.n is now the active member
    // at this point, reading from s.s or s.c is undefined behavior. but most compiler define it
    std::cout << std::hex << "s.n = 0x" << s.n << '\n';
    std::cout << std::hex << "s.s[0] = 0x" << s.s[0] << '\n';
    std::cout << std::hex << "s.s[1] = 0x" << s.s[1] << '\n';
    std::cout << std::hex << "s.c = 0x" << int(s.c) << '\n';
    //let's see how memory looks like
    char *p = (char *)&s;
    cout << std::hex << int(*p++) << endl;
    cout << std::hex << int(*p++) << endl;
    cout << std::hex << int(*p++) << endl;
    cout << std::hex << int(*p) << endl;

    s.s[0] = 0x0011; // s.s is now the active member
    // at this point, reading from n or c is UB but most compilers define it
    std::cout << "s.c is now " << +s.c << '\n' // 11 or 00, depending on platform. 11 for x86 little-endian
              << "s.n is now " << s.n << '\n'; // 12340011 or 00115678. 12340011 for x86 litter endian
}
