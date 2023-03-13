#include <iostream>
using namespace std;

class base {
public:
    void output() {
        cout << getVal() << endl;
    }

private:
    int getVal()
    {
        return val_;
    }
    int val_{1};
};

class derived : public base {
    int val_{2};
};

int main() {
    derived d;
    d.output(); // 注意： 打印1. 二不是2.
}
