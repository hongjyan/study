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

private:
    string s_{"1"}, s2_{"11"}, s3_{"111"};
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
        return s3_;
    }

private:
    string s_{"2"}, s2_{"22"}, s3_{"222"};
};

int main() {
    derived d;
    d.output();
}
