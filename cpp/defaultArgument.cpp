#include <cstdio>

void print(int a, int b=10, int c=11, int d=12)
{
    printf("a=%d, b=%d, c=%d, d=%d\n", a, b, c, d);
}

int main()
{
    print(9);
    print(9, , 21,);
    return 0;
}