#include <iostream>
using namespace std;

class Base {
public:
    void overload(int a) {
	cout<<"overload(int)"<<endl;
    }

    void overload(int a) const {
        cout << "const void overload(int)" << endl;
    }

    void overload(float a) {
	cout<<"overload(float)"<<endl;
    }
/*wrong, since return type isn't part of function protype.    
    float overload(float a) {
	cout << "float overload(float a)" << endl;
    }
*/

    virtual void v(void) {
	cout<<"v(void) in base class"<<endl;
    }

    virtual Base& v1(void) {
	cout<<"v1(void) in base class"<<endl;
	return *this;
    }

    virtual Base* v2(void) {
	cout<<"v2(void) in base class"<<endl;
	return this;
    }
    
    virtual void v2(int) {
        cout << "V2(int) in base class" << endl;
    }

    virtual Base v3(void) {
	cout << "v3(void) in base class" << endl;
	return *this;
    }

    virtual void hide1(int i) {
	cout << "void hide1(int) in base class" << endl;
    }

    void hide2(void) {
	cout << "void hide2(void) in base class" << endl;
    }

    void hide2(int) {
	cout << "\"void hide2(int)\"" << endl;
    }

    void only_exist_in_base() {
	cout << "\"void only_exist_in_base()\" in base class" << endl;
    }

};

class Derived : public Base {
public:
    void overload(int c) {  // name overload, but will hide, if it is not virtual.
	cout<<"overload(char) "<<c<<" in Derived class"<<endl;
    }

    void v(void) {
	cout<<"v(void) in Derived class"<<endl;
    }

    Derived& v1(void) {
	cout<<"v1(void) in Derived class"<<endl;
	return *this;
    }

    Derived* v2(void) {
	cout<<"v2(void) in Derived class"<<endl;
	return this;
    }

    void v2(int) {
        cout << "void v2(int) in Derived class" << endl;
    }


/*wrong, since the difference of return valus that one is object of base, the other is object of dervived can not override.
If there is same name function of derived class than base class.  Derived function either overrides base function or hide base function.
Override only happens when the two has "the same prototype" and "return type(when return type is reference or pointer type, it doesn't matter type is base type or derived type)". 
Hide only happens at 1. when function is virtual, the two has differernt prototype. 2. when function is not virtual, derived fuction will hide base function no matter function prototype&return type.      
    Derived v3(void) {
	cout << "v3(void) in derived class" << endl;
	return *this;
    }
*/

    void hide1() {
	cout << "void hide1() in Derived class" << endl;
    }

    void hide2(void) {
	cout << "\"void hide2(void)\" in Derived class" << endl;
    }

    void hide2(float f) {
	cout << "\"void hide2(float f)\" in Derived class" << endl;
    }

    void derived_specific(void) {
	cout << "\"void Derived_specific(void)\" in Derived class" << endl;
    }
};

int main() {
    Derived d;
    Base *p = &d;
    Derived *pd = &d;
    p->overload(1); //overload(int)
    //p->overload(1.0); //call of overloaded ‘overload(ouble)’ is ambiguous
    p->overload(1.0f); //overload(float)

//    p->derived_specific();  //error: ‘class Base’ has no member named ‘derived_specific’s, base class pointer/reference can't call derived non-virtual function.

    p->v(); //v(void) in Derived class. override. 
    p->v1(); //v1(void) in Derived class. override though return value is different. One is reference to base, the other is reference to derived
    p->v2(); //v2(void) in Derived class. override though return value is different. One is pointer to base, the other is pointer to derived.

    p->hide1(1);  //void hide1(int) in base class
//    p->hide1();  //error: no matching function for call to ‘Base::hide1()’. no matching function for call to Base::v_hide(),\
		   //since those hide1() have different parameters, so dynamic bonding won't \
		//takes effect, so p->hide1() will use hide1() of base class since p is base type, but there isn't hide1(int) in base class, \
		//so this line is wrong.
    pd->overload('f');  //overload(char) 102 in Derived class
    pd->overload(1); //overload(char) 1 in Derived class. overload(char) of Derived class hides function of Base.
//    pd->overload();  //no matching function for call to ‘Derived::overload()’ .overload in derived class hide function of base class.

//    pd->hide1(1); // error: no matching function for call to ‘Derived::hidede(int)’ .ff .dynamic bonding won't be take effect, same as p->hide1().
    pd->hide1(); //void hide1() in Derived class
    
    pd->hide2(); //"void hide2(void)" in Derived class
    pd->hide2(1); //"void hide2(float f)" in Derived class
    pd->only_exist_in_base(); //"void only_exist_in_base()" in base class. No same name function in derived class, so no hide, so function of base works.
}
    
