#include <cstdio>

class Hei {
public:
    Hei() { printf("constructor of Hei\n"); }
    Hei(Hei& h) {
        printf("copy-constructor of Hei\n");
    }
    Hei& operator=(const Hei& src) {
        printf("operator= of Hei\n");
        return *this;
    }
    Hei getHei() {
        return *this;
    }
   
};

int main() {
    Hei hei;
    Hei hei2 = hei;
    Hei hei3 = hei.getHei(); //error: no matching function for call to 'Hei::Hei(Hei)'. 
}

