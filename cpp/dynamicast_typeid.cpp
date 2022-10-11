#include <iostream>
#include <typeinfo>
#include <string>
using namespace std;


class Base {
friend bool operator==(const Base &lhs, const Base &rhs);
public:
  Base() = default;
  Base(int age, string name): _age(age), _name(name) {}
  virtual void foo() {};

  void say_hello() {
    cout << "hello, I am Base" << endl;
  }

protected:
  bool equal(const Base &rhs) const {
    return _age == rhs._age && _name == rhs._name;
  }

private:
  int _age;
  string _name;
};

bool operator==(const Base &lhs, const Base &rhs) {
  return typeid(lhs) == typeid(rhs) && lhs.equal(rhs);
}


class Derived: public Base {
public:
  Derived() = default;
  Derived(int age, string name, string phone): Base(age, name), _phone(phone) { }
  virtual void foo() {}
  void say_hello() {
    cout << "hello, I am Derived" << endl;
  } 

protected:
  bool equal(const Base &rhs) const {
    const Derived &d = dynamic_cast<const Derived&>(rhs);
    return Base::equal(rhs) && _phone == d._phone;
  }

private:
  string _phone;
};


int main() {
  //cast successfully
  Derived d;
  Base *p = &d;
  p->say_hello();

  if (Derived *pd = dynamic_cast<Derived*>(p)) {
    pd->say_hello();
  }
  else {
    p->say_hello();
  }
    

  //fail cast
  Base b;
  Base *pb = &b;
  if (Derived *pd = dynamic_cast<Derived*>(pb)) {
    pd->say_hello();
  } 
  else {
    pb->say_hello();
  }
  

  //typeid
  int i = 1234;
  typeid(1)==typeid(i) ? cout << std::hex << i  << endl : cout << "neq" << endl;
  cout << (typeid(*p) == typeid(d)) << endl; //compare type of two expression
  cout << (typeid(*p) == typeid(Derived)) << endl;  //compare type of expression and another type
    //ok to *p even p is null when p points to no-polymophic type
  int *p1 = nullptr;
  cout << typeid(*p1).name() << endl;

    //will throw exception when p points to polymophic type
  pb = nullptr;
  try {
    cout << typeid(*pb).name() << endl;
  } 
  catch (std::bad_typeid &e) {
    cout << e.what() << endl;
  }

  //typeid and dynamic_cast example from book
  Base b1(12, "xiaoming");
  Derived d1(12, "xiaoming", "13855607898");
  Derived d2(12, "xiaoming", "13855607898");
  cout << (b1 == d1) << endl;
  cout << (d1 == d2) << endl;
  
  return 0;
}
