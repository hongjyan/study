#include <string>
#include <iostream>

int main()
{
    
    std::string str = "This is a string tail";
    str.erase(str.find_last_not_of(" ") + 1);
    std::cout << str << std::endl;
    str.erase(str.rfind(" "));
    std::cout << str << std::endl;
    return 0;
}
