#include <string>

class Stub {
public:
    Stub() {}
    static void setName(const std::string&);
    static std::string getName();
    static void handler(std::string);
private:
    static std::string _name;
};
