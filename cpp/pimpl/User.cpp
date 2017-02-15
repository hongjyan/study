#include "NamespaceProxy.hpp"
#include <iostream>

int main() {
	NamespaceProxy nsp;
	nsp.setNamespace("xiaoming");
	std::cout << nsp.getNamespace() << std::endl;
	nsp.setNamespace("xiaoli");
	std::cout << nsp.getNamespace() << std::endl;
	return 0;
}
