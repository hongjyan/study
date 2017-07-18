#include <string>
#include <vector>
#include <iostream>
#include <boost/algorithm/string/regex.hpp>


void splitOutput(const std::string& output, const std::string& delim, std::vector<std::string>& outSections)
{
    size_t last=0, cur=0;
    cur = output.find(delim, cur);
    int len = 0;
    std::cout << output.size() << std::endl;

    while (std::string::npos != cur) {
        last = cur;
        cur += delim.size();
        cur = output.find(delim, cur);
        len = (std::string::npos == cur) ? std::string::npos : cur-last;        
        
        outSections.push_back(output.substr(last, len));
    }
}

//stackoverflow example
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

template<typename Out>
void split(const std::string &s, char delim, Out result) { //delim is char.
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}
//

int main()
{
    std::string output = "\r\n"
"    session name       : yhjtest\r\n"
"    src address        : 10.56.66.63\r\n"
"    dst address        : 1.2.3.4\r\n"
"    owner              : /MMN-0\r\n"
"    vrf name (ID)      : default (0)\r\n"
"    detect mult        : 4\r\n"
"    tx interval (msec) : 1000\r\n"
"    rx interval (msec) : 1000\r\n"
"    hop type           : single\r\n"
"    reference id       : 0\r\n"
"    dscp               : 34\r\n"
"    admin-state        : Enabled\r\n"
"\r\n"
"    session name       : yhjtest2\r\n"
"    src address        : 10.56.66.63\r\n"
"    dst address        : 1.2.3.5\r\n"
"    owner              : /MMN-0\r\n"
"    vrf name (ID)      : default (0)\r\n"
"    detect mult        : 4\r\n"
"    tx interval (msec) : 1000\r\n"
"    rx interval (msec) : 1000\r\n"
"    hop type           : single\r\n"
"    reference id       : 0\r\n"
"    dscp               : 34\r\n"
"    admin-state        : Enabled\r\n";

    std::vector<std::string> sections;
    std::string delim = "session name       :";
    splitOutput(output, delim, sections);

    for (std::vector<std::string>::const_iterator it=sections.begin(); it!=sections.end(); ++it) {
        std::cout << "-------------------------------------------------" << std::endl;
        std::cout << *it ;
    }
    
    std::vector<std::string> result;
    boost::split_regex(result, output, boost::regex("session name[\\s\\t]+:") );
    for (std::vector<std::string>::const_iterator it=result.begin(); it!=result.end(); ++it) {
        std::cout << "*******************************************************" << std::endl;
        std::cout << *it ;
    }

    return 0; 
}
