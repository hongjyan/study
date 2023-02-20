#include <string>
#include <functional>
#include <uchar.h>

template <class T>
class Rule
{
public:
    using Function = std::function<bool(const T&)>;

    Rule(Function func) : func_(std::move(func)) {}

    // 不同类型的模板实例间的转换， 前提为U可以转换为T， 或者U==T.
    // U->T的转换发生再下面的operator()中
    template <class U>
    Rule(const Rule<U>& other): Rule([&other] (const T& item) { return other(item); }) {}

    bool operator()(const T& item) const
    {
        return func_(item);
    }

private:
    Function func_;
};

bool szgt6(const size_t& sz)
{
    return sz>6 ? true:false;
}

bool igt6(const int& i)
{
    return i>6 ? true:false;
}

bool sgt6(std::string s)
{
    return s.size()>6 ? true:false;
}
