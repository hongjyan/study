#include <sstream>
#include <string>
#include <cstdio>

int main() {
    std::string s = "interfaces in default instance:\r\n"
"-------------------------------\r\n"
"\r\n"
"  internal (Ethernet interface)\r\n"
"    VRF name(ID)        : default (0)\r\n"
"    owner               : /MMN-0\r\n"
"    MAC                 : fa:16:3e:c8:f0:22\r\n"
"    MTU                 : 1500\r\n"
"    admin state         : up\r\n"
"    IPv4 forwarding     : no\r\n"
"    IPv6 forwarding     : no\r\n";

    std::istringstream iss(s);
    std::string temp;
    while (iss >> temp) {
	printf("%s\n", temp.c_str());
    }
    return 0;

}
