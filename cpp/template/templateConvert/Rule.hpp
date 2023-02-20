#include <string>
#include <functional>
#include <uchar.h>

template <class T>
class Rule
{
public:
    using Function = std::function<bool(const T&)>;

    Rule(Function func) : func_(std::move(func)) {}

    //?????????????, ???U?????T, ??U==T. 
    //U->T?????????operator()?
    template <class U>
    Rule(const Rule<U>& other): Rule([&other] (const T& item) { return other(item); }) {}

    bool operator()(const T& item) const //?????const? ???????
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
