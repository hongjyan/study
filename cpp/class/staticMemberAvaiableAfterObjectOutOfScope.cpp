#include <cstdio>

class Foo {
public:
    static int i;
};

int Foo::i = 10;

int main() {
    if (true) {
        Foo f;
    }

    printf("Foo::i is %d\n", Foo::i);
}
