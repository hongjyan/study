#include "stub.hpp"
   
void Stub::setName(const std::string& name) {
       Stub:: _name = name;
}

void Stub::handler(std::string name) {
        setName(name);
}

std::string Stub::getName() {
    return Stub::_name;
}


std::string Stub::initName() {
    std::string name = "init";
    return name;
}

//Stub::initName();
std::string Stub::_name = initName();
