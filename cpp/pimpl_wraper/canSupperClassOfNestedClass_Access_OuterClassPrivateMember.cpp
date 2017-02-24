#include <iostream>

class Outer {
public:
	Outer(int num): age(num) {}
	class Nested;

private:
	int age;
};

//summary: supper of nested class can not access nestedClass's outerClass's private member.
class Supper {
public:
	static int getAge(const Outer& outer) {
//		return outer.age;
		return 0;
	}
};


class Outer::Nested : public Supper {
public:
	static int getAge(const Outer& outer) {
		return outer.age;
	}
};


int main() {
	Outer outer(12);
	std::cout << Outer::Nested::getAge(outer) << std::endl;
	std::cout << Supper::getAge(outer) <<std::endl;
	return 0;
}
