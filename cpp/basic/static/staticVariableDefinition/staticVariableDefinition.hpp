#include <string>

class Foo {
public:
	std::string getSex();
	static void setSex(const std::string& sex);
	static std::string name_;
	

private:
	static std::string sex_;
};
