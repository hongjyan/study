#include "staticVariableDefinition.hpp"

std::string Foo::getSex() {
	return sex_;
}

void Foo::setSex(const std::string& sex) {
	sex_ = sex;
}

//Foo::setSex("male");

std::string Foo::sex_ = "male";
std::string Foo::name_ = "Fun";
