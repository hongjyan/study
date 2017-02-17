#include <string>
#include <iostream>

class Foo {
public:
	static void sayHi() {
		std::cout << "Hi" << std::endl;
	}
private:
	Foo(const std::string& name): name_(name) {}
	std::string name_;
};


int main() {
	Foo::sayHi();
	return 0;
}
