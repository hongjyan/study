#include "stub.hpp"
   
void Stub::setName(const std::string& name) {
        _name = name;
}

void Stub::handler(std::string name) {
        setName(name);
}

std::string Stub::getName() {
    return _name;
}
