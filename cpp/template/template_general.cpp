#include <iostream>
using namespace std;


#include <type_traits>
#include <typeinfo>
#ifndef _MSC_VER
#   include <cxxabi.h>
#endif
#include <memory>
#include <string>
#include <cstdlib>

template <class T>
std::string
type_name()
{
    typedef typename std::remove_reference<T>::type TR;
    std::unique_ptr<char, void(*)(void*)> own
           (
#ifndef _MSC_VER
                abi::__cxa_demangle(typeid(TR).name(), nullptr,
                                           nullptr, nullptr),
#else
                nullptr,
#endif
                std::free
           );
    std::string r = own != nullptr ? own.get() : typeid(TR).name();
    if (std::is_const<TR>::value)
        r += " const";
    if (std::is_volatile<TR>::value)
        r += " volatile";
    if (std::is_lvalue_reference<T>::value)
        r += "&";
    else if (std::is_rvalue_reference<T>::value)
        r += "&&";
    return r;
}



template <typename T> void func(T val) {
    cout << type_name<decltype(val)>() << endl;
}

template <typename T> void func_rr(T&& val) {
    cout << type_name<decltype(val)>() << endl;
}

///////////////////////practice//////////////////////////////////

void f(int &v) {
    ++v;
}

void g(int&& v) {
   ++v;
}

template <typename F, typename T>
void temp_late(F f, T t) {
  f(t);
}

template <typename F, typename T>
void temp_late2(F f, T&& t) {
  f(t);
}

template <typename F, typename T>
void temp_late3(F f, T&& t) {
//compared to t as argument of f, lead to lvalue always.
//for std::forward<T>.
// T = "type of argument of temp_late3". 
// and std::forward<T>  = T&&.
// So, std::forward<T> = "type of argument of temp_late3" + &&.
// In detail, if template_late3's argument is rvalue, e.g. 42, then std::forward<T> is e.g. int&& &&--->int&&. if template_late3's argument is lvalue, e.g. int i, the std::forward<T> is e.g. int& &&---> int & 
  f(std::forward<T>(t));  
}

int main() {
    const int i = 10;
    cout << type_name<decltype(i)>() << endl;  //int const
    func(i);   //int. const is needless in func(const int i), since parameter i is copy-transfer
    const int &j = i;
    func(j);   //template can not defer reference.
    cout << endl << endl << endl;

    //******************/
    //use rvalue-reference parameter to keep argument cv,reference properties.
    //******************/ 
    func_rr(i); //int const&. since reference-fold. indicate that we can use rvalue-reference to keep argument reference in template type deferring.
    func_rr(j); //int const&. since reference-fold.
    cout << endl << endl << endl;

    //more practice
    int h = 10;
    int &v = h;
    f(v);
    cout << "v is " << v << endl;
    
    temp_late(f, v); //can not change v
    cout << "v is " << v << endl;

    temp_late2(f, v); //will change v
    cout << "v is " << v << endl;
    
    g(42);
    //temp_late2(g, 42); //error: cannot bind 'int' lvalue to 'int&&' 
    cout << endl << endl << endl;


    //std::forward<T>(arg) usage
    cout << type_name<decltype(std::forward<int>(v))>() << endl; //int&&
    cout << type_name<decltype(std::forward<int>(42))>() << endl; 
    cout << type_name<decltype(std::forward<int&>(v))>() << endl; //int& &&---> int&
    //cout << type_name<decltype(std::forward<int&>(42))>() << endl; //disallow bind 42 to int&.
    cout << type_name<decltype(std::forward<int&&>(v))>() << endl; //int&& && ---> int &&
    cout << type_name<decltype(std::forward<int&&>(42))>() << endl;
    temp_late3(g, 42);
    return 0;
}
