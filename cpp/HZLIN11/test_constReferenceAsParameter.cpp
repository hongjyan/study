#include <cstdio>
#include <string>


class Foo {
public:
    Foo(const std::string& name): _name(name) { }

    std::string getName() {
        return _name;
    }
private:
    const std::string _name;
    //const std::string& _name;
};

int main() {
    std::string name = "game";
    Foo f(name);
    printf("f._name is %s\n", f.getName().c_str());

    name = "fun";
    printf("f._name is %s\n", f.getName().c_str());


}
    
