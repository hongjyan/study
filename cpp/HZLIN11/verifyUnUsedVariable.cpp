#include <string>

class Foo {
public:
    Foo(bool flag): named(flag) {};

    void InitialName(const char* name) {
        if (named) {
            mName = name;
        }
    }

private:
    bool named;
    std::string mName;
};

int main() {
    Foo f(false);
}
