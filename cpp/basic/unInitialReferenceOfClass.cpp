#include <string>
#include <iostream>


class Bar {
public:
	Bar(std::string& beer): beer_(beer) {}
	std::string getBeer() {
		return beer_;
	}
private:
	std::string beer_;
};


class Foo {
public:
	Foo(const std::string& name): name_(name) {}

	std::string getBeer() {
		return bar_.getBeer();
	}
private:
	std::string name_;
	Bar& bar_;
};

int main() {
	Foo foo("xiaoming");
	std::cout << foo.getBeer() << std::endl;
	return 0;
}
