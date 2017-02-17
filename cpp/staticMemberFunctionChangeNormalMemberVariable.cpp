#include <string>
#include <iostream>

class Foo {
public:
    //can not change non-static variable member via static member function
	/*
	static std::string changeName(std::string& name) {
		name_ = name;
		return name_;
	}
    */

    //do not know why this is OK
    static std::string changeName(Foo& foo, const std::string& name) {
		foo.name_ = name;
		return foo.name_;
	}
private:
	std::string name_;
};

int main() {
	Foo foo;
	//std::cout << foo.changeName("xiaoming") << std::endl;
	std::cout << Foo::changeName(foo, "xiaoming") << std::endl;
	return 0;
}
