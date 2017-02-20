#include "../trim_string.h"
std::string trim_string(const std::string &str, const std::string &whitespace) {
    typedef std::string::size_type index;
    index strBegin = str.find_first_not_of(whitespace);
    index strEnd = str.find_last_not_of(whitespace);
    return str.substr(strBegin, strEnd-strBegin+1);
}
