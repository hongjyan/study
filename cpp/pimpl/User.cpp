#include "Context.hpp"
#include <iostream>

int main() {
	Context ctx;
	ctx.setNamespace("xiaoming");
	std::cout << ctx.getNamespace() << std::endl;
	ctx.setNamespace("xiaoli");
	std::cout << ctx.getNamespace() << std::endl;
	return 0;
}
