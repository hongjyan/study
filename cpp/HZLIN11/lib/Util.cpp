#include <string>
#include <sstream>
#include <iomanip>

std::string toHexString(const unsigned char *buffer, unsigned int len)
{
    std::ostringstream s;
    for (unsigned int i = 0; i < len; i++)
        s << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << static_cast<unsigned int> (buffer[i]) << " ";
    return s.str();
}

