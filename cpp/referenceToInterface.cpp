//YES, IT CAN
#include <iostream>
using namespace std;

namespace Simple {
    struct IFoo {
        virtual void print() const = 0;
    };

    class Foo : public IFoo {
    public:
        void print() const { cout << "I am Foo" << endl; }
    };

    class FooDecorator : public IFoo {
    public:
        FooDecorator(const IFoo& foo): foo_(foo) {}
        void print() const { 
            foo_.print();
            cout << "I am FooDecorator" << endl; }
    private:
        const IFoo& foo_;
    };
}

//sigleton pattern + decorator pattern using reference to act polymophsim
namespace Singleton {
    struct IFoo {
        virtual void print() const = 0;
    };

    class Foo : public IFoo {
    public:
        void print() const { cout << "I am " << str_ << endl; }
        static Foo& getInstance() {
            static Foo foo;
            return foo;
        }
        const Foo& setStr(const string& str) {
            str_ = str;
            return *this;
        }
    private:
        Foo() = default;
        Foo(const Foo&) = delete;
        Foo& operator=(const Foo&) = delete;
        string str_;
    };

    class FooDecorator : public IFoo {
    public:
        FooDecorator(const IFoo& foo): foo_(foo) {}
        void print() const { 
            foo_.print();
            cout << "I am " << x_ << endl; }
        const FooDecorator& setInt(int x) {
            x_ = x;
            return *this;
        }
        
    private:
        const IFoo& foo_;
        int x_;
    };
}



int main () {
    Simple::FooDecorator(Simple::Foo()).print();

    cout << "--------------------" << endl;
    auto& foo = Singleton::Foo::getInstance();
    foo.setStr("hello").print();
    Singleton::FooDecorator(foo).setInt(11111).print();

}