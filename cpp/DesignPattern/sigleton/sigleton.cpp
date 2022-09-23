/*****https://stackoverflow.com/questions/21395459/how-to-pass-argument-in-a-singleton
 *****/

#include <string>
#include <iostream>
#include <stdexcept>
#include <memory>
using namespace std;

class Questionnary
{
  std::string _str;

  static Questionnary& getInstanceImpl(std::string* const s = nullptr)
  {
    static Questionnary instance{ s };
    return instance;
  }

  Questionnary(std::string* const s)
    : _str{ s ? move(*s) : std::string{} } // employ move ctor
  {
    if (nullptr == s)
      throw std::runtime_error{ "Questionnary not initialized" };
  }

public:
  static Questionnary& getInstance()
  {
    return getInstanceImpl();
  }
  static void init(std::string s) // enable moving in
  {
    getInstanceImpl(&s);
  }

  void print() {
    cout << _str << endl;
  }

  Questionnary(Questionnary const&) = delete;
  void operator=(Questionnary const&) = delete;
};



class MySigleton {
private:
    string str_;

    MySigleton() = default;
    MySigleton(const MySigleton&) = delete;
    MySigleton& operator=(const MySigleton&) = delete;

public:
    static MySigleton* getInstance() {
        static MySigleton* mySigleton_ = nullptr;
        if (!mySigleton_) return new MySigleton();
        return mySigleton_;
    }

    MySigleton* set(const string& str) {
        str_ = str;
        return this;
    }  
    void print() { cout << str_ << endl; } 
};

class MySigleton2 {
private:
    string str_;
    static shared_ptr<MySigleton2> mySigleton2_;

    MySigleton2() = default;
    MySigleton2(const MySigleton2&) = delete;
    MySigleton2& operator=(const MySigleton2&) = delete;

public:
    static shared_ptr<MySigleton2> getInstance() {
        if (!mySigleton2_) mySigleton2_ = make_mySigleton2();
        return mySigleton2_;
    }

    shared_ptr<MySigleton2> set(const string& str) {
        str_ = str;
        return mySigleton2_;
    }  
    void print() { cout << str_ << endl; } 
};

struct DerivedSigleton2 : public MySigleton2 {};
std::shared_ptr<MySigleton2> make_mySigleton2()
{
    return std::make_shared<DerivedSigleton2>();
}

int main() {
    // Questionnary::getInstance().print(); //will throw runtime error since instance now initialized.

    Questionnary::init("foo");
    Questionnary::getInstance().print();

    Questionnary::init("bar");
    Questionnary::getInstance().print(); //output will be foo

    cout << "---------------" << std::endl;
    auto x = MySigleton::getInstance();
    x->print();
    x->set("foo")->print();
    x->set("bar")->print();

    cout << "---------------" << std::endl;
    make_mySigleton2();

}