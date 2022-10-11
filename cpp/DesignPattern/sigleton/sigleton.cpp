/*****https://stackoverflow.com/questions/21395459/how-to-pass-argument-in-a-singleton
 *****/

#include <string>
#include <iostream>
#include <stdexcept>
#include <memory>
using namespace std;

class Questionnary
{
  private:
    std::string str_;
    char c_;
    Questionnary(const std::string& s, char c): str_(s), c_(c) {}
 
public:
  static Questionnary& getInstance(const std::string& s, char c)
  {
    static Questionnary instance{s, c}; //static变量只初始化一次。
    instance.str_ = s; //但是可以赋值多次
    cout << instance.str_  << " " << instance.c_ << endl;
    return instance;
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

int main() {
    Questionnary::getInstance("foo", 'a'); //"foo a"
    Questionnary::getInstance("bar", 'b'); //"bar a"

    cout << "---------------" << std::endl;
    auto x = MySigleton::getInstance();
    x->print();
    x->set("foo")->print();
    x->set("bar")->print();

    return 0;
}