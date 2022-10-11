#include <iostream>

class  Outer
{
private:
    /* data */
public:
     Outer(/* args */);
    ~ Outer();
    template<typename T>
    class Inner
    {
    public:
        Inner(T t) : t_(t) {}
        void print() { std::cout << t_ << std::endl; }
    private:
        T t_;
    };
};

 Outer:: Outer(/* args */)
{
}

 Outer::~ Outer()
{
}

template<typename T>
class Encloser
{
public:
    struct A
    {
        T t;
    };

    struct B
    {
        T t;
    };

    void doNothing() {}
};


template<typename T, template<typename> class TT, typename Inner>
Inner generateTemplateInnerClass2(T t)
{
    TT<T> e;
    e.doNothing();
    return Inner{t};
}

int main()
{
    Outer outer;
    Outer::Inner<int> inner(12);
    inner.print();

    std::cout << generateTemplateInnerClass2<int, Encloser, Encloser<int>::A>(10).t << std::endl;
}
