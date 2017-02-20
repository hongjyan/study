#include <iostream>
#include "staticVariableDefinition.hpp"

int main() {
	std::cout << Foo::name_ << std::endl;
	Foo::setSex("female");
	Foo foo;
	std::cout << foo.getSex() << std::endl;
	return 0;
}	
