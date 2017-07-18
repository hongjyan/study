#include <cstdio>
#include <sstream>
#include <string>
#include <map>

#define LINE_MAXSIZE 65535

    std::string trimstring(const std::string& str, const std::string& whitespace = " ") {
	int beg = str.find_first_not_of(whitespace);
	if (beg == std::string::npos)
	    return "";
	int end = str.find_last_not_of(whitespace);
	int range = end - beg + 1;
	return str.substr(beg, range);
    }


    std::map<std::string, std::string> OutputToMap(const std::string& output) {
	std::istringstream iss(output);
	std::string line, key, value;
        char temp[LINE_MAXSIZE] = {0};
	std::map<std::string, std::string> ret;
	int pos;
	while (iss.getline(temp, LINE_MAXSIZE)) {
            line = temp; 
	    if (std::string::npos != (pos = line.find_first_of(":")) && 
                std::string::npos != (pos = line.find(" ", pos+1, 1))) {
		key = trimstring(line.substr(0, pos-1));
		value = trimstring(line.substr(pos, std::string::npos));
                ret[key] = value;
	    }
	}
        return ret;
    }


int main() {
    std::string output = "interfaces in default instance:\r\n"
"-------------------------------\r\n"
"\r\n"
"  internal (Ethernet interface)\r\n"
"    VRF name(ID)        : default (0)\r\n"
"    owner               : /MMN-0\r\n"
"    MAC                 : fa:16:3e:c8:f0:22\r\n"
"    MTU                 : 1500\r\n"
"    admin state         :up\r\n"
"    IPv4 forwarding     : no\r\n"
"    IPv6 forwarding     : no\r\n";

    std::map<std::string, std::string> m = OutputToMap(output);
    std::map<std::string, std::string>::const_iterator it = m.begin();
    while ( it != m.end()) {
        printf("%s:%s\n", it->first.c_str(), it->second.c_str());
        it++;
    }
    return 0;
}

