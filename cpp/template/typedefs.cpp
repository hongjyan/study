#include <vector>
#include <iostream>

template<typename T>
struct Foo
{
typedef std::vector<T> TVec;
TVec f(T t) {
    TVec tvec; 
    tvec.push_back(t);
    return tvec;
}
};

int main()
{
    Foo<int> iFoo;
    for (auto x : iFoo.f(1)) {
        std::cout << x << std::endl;
    }
    return 0;
}
