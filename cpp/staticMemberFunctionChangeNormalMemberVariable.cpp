#include <string>
#include <iostream>

class Foo {
public:
    //can not change non-static variable member via static member function. The reason is that static-function maybe exist 
    // before instance creation.
	/*
	static std::string changeName(std::string& name) {
		name_ = name;
		return name_;
	}
    */

    //OK here since Foo was created, so static function can access this non-static member of it.
    //Think static function member as a global function.
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
