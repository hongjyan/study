#include <iostream>
using namespace std;


template <class T> 
class Foo {
    //template with all intanceses are friend
    template <class type> friend class allInstancesClass;  //insert "friend" to normal class template declaration
    template <size_t N, typename type> friend void allIntancesFunction(type &t);  //insert "friend" to normal function template declaration.

    //normal function/class is friend
    friend class normal_class;
    friend void f(int, float);

    //specific instance of template is friend
    //template <class type> friend class singleInstanceClass<int>;   //wrong, must declare template firstly
    template <class type> class singleInstance;
    friend class singleInstance<int>;
    //template <size_t N, typename type> friend int template_f<12>(const int &); //wrong, must declare template firstly
    template <size_t N, typename type> int template_f(const type &);
    friend int template_f(const int &); 

    //specific instances of template are friend
    //template <class T> class typeInstancesClass;  //wrong, can't be the same name with T
    template <class Type> class typeInstancesClass;
    friend class typeInstancesClass<T>;
    template <size_t N, class Type> void type_f(const Type &);
    friend void type_f<78>(const T &);
};

template <class T> 
class Bar {
};


int main() {
}
