#include <iostream>
#include <vector>
#include <ostream>

template<typename Ret, typename... Args>
struct FunctionPointer {
    using fp_type = Ret (*)(Args...);
 
    FunctionPointer(fp_type x)
        : func_(x)
    {}
 
    Ret operator()(Args&&... args) {
        return func_(std::forward<Args>(args)...);
    }
 
    fp_type func_;
};
template<typename Ret, typename... Args>
FunctionPointer<Ret, Args...> make_function_pointer(Ret (*func)(Args...))
{
    return {func};
}



template<void (*FuncPtr)()> 
struct Func2Type
{
    void operator()() { FuncPtr(); }
};
void Hello() { std::cout << "Hello" << std::endl; }
void World() { std::cout << "world" << std::endl; }




template<typename Ret, typename... Args>
Ret add(Args... args) {
    return (... + args);
}
template<typename It>
auto multiple(It beg, It end) {
    auto ret = *beg;
    while(++beg != end) {
        ret *= *beg;
    }
    return ret;
}




int main()
{
    Func2Type<Hello> test1;
    Func2Type<World> test2;
    test1();
    test2();

    //这么做的好处是什么？ 这个模板可以作为函数参数如某个handler， 这样得话， 可以实现不同类型的handler？
    auto fn = make_function_pointer(add<int, int, int>);
    auto fn2 = make_function_pointer(add<double, double, double, double>);
    std::cout << fn(1, 2) << std::endl;
    std::cout << fn2(1.2, 3.4, 5.6) << std::endl;

    //普通函数指针实现
    int (*pfunc)(int, int) = add<int, int, int>;
    using pfunc2 = int (*)(int, int);
    pfunc2 p2 = add;
    std::cout << p2(1, 2) << std::endl;
    typedef int (*pfunc3)(int, int);
    pfunc3 p3 = add;
    std::cout << p3(1, 2) << std::endl;

    //不知道为啥写下面这个
    std::vector<int> v{1, 2, 3};
    std::cout << multiple(v.begin(), v.end()) << std::endl;
}
 