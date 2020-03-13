#include <cstdio>

int main() {
//initial, 2015.7.22
    int a[] = {1, 2, 3, 4};
    int *pa = a;
    int (*ptr) [4] = &a;
    printf("a is %p\n", a);
    printf("&a is %p\n", &a);
    printf("pa is %p\n", pa);
    printf("&pa is %p\n", &pa);
    printf("ptr is %p\n", ptr);
    printf("&ptr is %p\n", &ptr); 

    const char *s = "funny";
    printf("s address is %p, s is %s, &s is %p, &s[1] is %p, *s is %c\n",  s, s, &s, &s[1], *s);   
}

