#include <iostream>

class  Foo
{
private:
    /* data */
    int i_;
public:
    // friend bool operator==(const Foo&, const Foo&);
    Foo(int i);
    ~ Foo();
};

 Foo::Foo(int i) : i_(i)
{
}

 Foo::~ Foo()
{
}

// bool operator==(const Foo& lhs, const Foo& rhs)
// {
//     return lhs.i_ == rhs.i_;
// }

int main()
{
    Foo f1(1);
    Foo f2(1);
    auto equal = (f1 == f2);

    std::cout << "f1 == f2" << equal << std::endl;
}