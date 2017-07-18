#include <cstdio>

class Cat {
public:
    Cat();
    void purr();
    ~Cat();
private:
    class CatImpl;
    CatImpl* pCatImpl;
};
