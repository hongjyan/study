#include <cinttypes>
#include <cstddef>
#include <iostream>
using namespace std;

int main() {
    std::string str;
    std::cin >> str;
    std::cout << str;
    std::cout << str.c_str();
    auto i = std::strtol(str.c_str(), nullptr, 0);
    std::cout << i << endl;
}
// 64 位输出请用 printf("%lld")