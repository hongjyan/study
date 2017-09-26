#include <cstdio>

class Cat {
public:

public:
    Cat() { _catImpl = new CatImpl(); }
    ~Cat() { delete _catImpl; }

    void purr() {
        _catImpl->purr();
    }

private:
    class CatImpl {
    friend class Cat;
        void purr() {
            printf("purrrr\n");
        }
    };
    CatImpl* _catImpl;

};



int main() {
    Cat cat;
    cat.purr();
    //Cat::CatImpl catImpl;
}
    
