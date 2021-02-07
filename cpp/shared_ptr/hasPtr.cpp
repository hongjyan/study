#include <cstdio>
#include <string>
#include <iostream>
using namespace std;

class hasPtr {
private:
    string *p;
    int i;
    int *use;

public:
    friend void swap(hasPtr &lhs, hasPtr &rhs);

    hasPtr(const string &s = string()): p(new string(s)), i(0), use(new int(1)) {} 
    hasPtr(const hasPtr &obj): p(obj.p), i(obj.i), use(obj.use) {
        cout << "copy-constructor" << endl;
        ++*use;
    }

    hasPtr& operator=(const hasPtr &rhs) {
        cout << "operator=" << endl;
        if (this == &rhs) return *this;
        --*use;
        p = rhs.p;
        i = rhs.i;
        use = rhs.use;
        ++*use;
        return *this;
    }
    
    void free() { 
        if (nullptr != use && 0 == --*use) { delete p; delete use;}
    } 

    ~hasPtr() {
      free();
    }

    void print() {
        printf("my.p is %s, my use is %d\n", p->c_str(), *use);
    }

//move constructor
    hasPtr(hasPtr &&obj) noexcept : p(obj.p), i(obj.i), use(obj.use) { 
        cout << "move-constructor" << endl;
        obj.p = 0;
        obj.use = nullptr;
    }
//move operator=
    hasPtr& operator=(hasPtr &&rhs) {  //parameter is value than reference.
        cout << "move operator=" << endl;

        if (this == &rhs) return *this;
        
        free();
        p = rhs.p;
        use = rhs.use;
        i = rhs.i; 

        rhs.p = nullptr;
        rhs.use = nullptr;
        return *this;
    }
};

void swap(hasPtr &lhs, hasPtr &rhs) {
    using std::swap;
    swap(lhs.p, rhs.p);
    swap(lhs.i, rhs.i);
}

int main() {
    hasPtr p1("happy");
    hasPtr p3(p1);
    hasPtr p2("hope");
    hasPtr p5("work");
    p5 = p1;
    hasPtr p4(p2);
    p1.print();
    p2.print();

    hasPtr p7 = std::move(p1);
    p7.print();
    p3.print();
    p7 = hasPtr("rvalue");
    p7.print();
}
