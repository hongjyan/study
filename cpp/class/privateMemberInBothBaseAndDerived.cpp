#include <iostream>
#include <string>
using namespace std;

class base {
public:
    base() : s_("1"), s2_("11"), s3_("111") {}

    void output() {
        // 同vGetStr3. 且这是标准用法， 因为通常将需要override的函数放到private下。
        cout << vGetStr() << endl;
        // 虽然vGetStr2在基类定义为virtual， 但是派生类没有override vGetStr2， 所以vGetStr2依然只返回基类s2.
        cout << vGetStr2() << endl;
        // 派生类重声明了s3, 所以派生类和基类各有一个s3, 派生类s3隐藏基类s3. 
        // 但由于getStr3为非virtual函数， 所以始终getStr3获得的为基类s3
        // vGetStr3返回this->s3
        cout << getStr3() << endl;
        cout << vGetStr3() << endl;
        // S4_ 被派生类继承， 而非hide， 所以s4在派生类和基类地址相同。其值始终为this->s4。
        cout << getStr4() << endl;
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

    string getStr4()
    {
        cout << __func__ << ": ";
        return s4_;
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
    derived() : s_("2"), s2_("22"), s3_("222") { s4_ = "2222";}

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
    string vGetStr4()
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
