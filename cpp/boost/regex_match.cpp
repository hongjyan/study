#include <boost/regex.hpp>
#include <string>
#include <vector>
#include <iostream>

int main() {
    const char *input = "24 bytes from 212.212.212.212: icmp_seq=4 ttl=64 time=0.989 ms";
    boost::regex expression(".*(icmp_seq=\\d+)\\s+(ttl=\\d+)\\s+(time=\\d+\\.\\d+)\\s+ms.*");

    //std::vector<std::string> matches;
    boost::cmatch matches;

    if (boost::regex_match(input, matches, expression)) {
        for (auto it=matches.begin(); it!=matches.end(); ++it) {
            std::cout << *it << std::endl;
        }
    }

    return 0;
}
