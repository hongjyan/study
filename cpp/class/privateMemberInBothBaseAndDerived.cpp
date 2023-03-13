#include <iostream>
#include <string>
using namespace std;

class base {
public:
    base() : s_("1"), s2_("11"), s3_("111") {}
    // 如果函数非virtual， 一律使用基类内容
    // 如果函数为virtual， 只有派生类重定义了，那么才使用派生类内容
    void output() {
        cout << vGetStr() << endl;
        cout << vGetStr2() << endl;
        cout << getStr3() << endl;
        cout << vGetStr3() << endl;
        cout << vGetStr4() << endl;
    }

private:
    virtual string vGetStr()
    {
        cout << __func__ << ": ";
        return s_;
    }

public:
    virtual string vGetStr2()
    {
        cout << __func__ << ": ";
        return s2_;
    }

    string getStr3()
    {
        cout << __func__ << ": ";
        return s3_;
    }
    virtual string vGetStr3()
    {
        cout << __func__ << ": ";
        return s3_;
    }

    virtual string vGetStr4()
    {
        cout << __func__ << ": ";
        return s4_;
    }

private:
    string s_{"1"}, s2_{"11"};
protected:
    string s3_{"111"};
protected:
    string s4_{"1111"};
};

class derived : public base {
public:
    derived() : s_("2"), s2_("22"), s3_("222") {}

private:
     string vGetStr()
    {
        cout << __func__ << ": ";
        return s_;
    }
public:
    string getStr3()
    {
        cout << __func__ << ": ";
        return s3_;
    }
    string vGetStr3()
    {
        cout << __func__ << ": ";
        cout << "s3 address is " << &s3_;
        cout << ", base::s3 address is " << &(base::s3_);
        return s3_;
    }

    //派生类没有声明s4_, 所以并没有hide基类的s4_, 而是继承基类s4_, 所以s4只有一份。
    virtual string vGetStr4()
    {
        cout << __func__ << ": ";
        cout << "s4 address is " << &s4_;
        cout << ", base::s4 address is " << &(base::s4_) << ". ";
        return s4_;
    }

private:
    string s_{"2"}, s2_{"22"}, s3_{"222"};
};

int main() {
    derived d;
    d.output();
}
