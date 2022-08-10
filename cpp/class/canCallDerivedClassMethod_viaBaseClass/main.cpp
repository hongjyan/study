#include <iostream>
#include <functional>

struct Base {
    virtual void func1() = 0;
    virtual ~Base() = default;
};

struct Derived : public Base {
    void func1() override {
        std::cout << "func1 in Derived" << std::endl;
    }

    void func2() {
        std::cout << "func2 in Derived" << std::endl;
    }
};

int main() {
    Derived d;
    std::reference_wrapper<Base> ref_d = std::ref(d);
    //ref_d.get().func2();
    ref_d.get().func1();
    return 0;
}