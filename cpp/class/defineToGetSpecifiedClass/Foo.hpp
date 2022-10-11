#define FooX1(y, c) Foo(1, y, c)

class Foo {
public:
    Foo(int x, int y, char c);

private:
    int x_, y_;
    char c_;
};

