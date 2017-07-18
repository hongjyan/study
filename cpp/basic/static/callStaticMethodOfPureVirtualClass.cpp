#include <string>
#include <iostream>

class Base {
public:
	Base(const std::string& name): name_(name) {};

	static void sayHi() { 
		std::cout << "Hi" << std::endl;
	}

	static std::string changeName(Base& base, const std::string& name) {
		base.name_ = name;
		return base.name_;
	}

	virtual void pureVirtual() = 0;

private:
	std::string name_;
};

class Derived : public Base {
public:
	Derived(const std::string& name): Base(name) {};

	void  pureVirtual() {
		std::cout << "I am Pure function" << std::endl;
	}
};


int main() {
	Base::sayHi();
	Derived derived("xiaoming");
	std::cout << Base::changeName(derived, "xiaoli") << std::endl;
	return 0;
}


