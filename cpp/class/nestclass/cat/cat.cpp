#include "cat.hpp"

class Cat::CatImpl {
friend class Cat;
    void purr() {
        printf("Purr\n");
    }
};

Cat::Cat() {
    pCatImpl = new CatImpl();
}

void Cat::purr() {
    pCatImpl->purr();
}

Cat::~Cat() {
    if (NULL != pCatImpl) delete pCatImpl;
}
