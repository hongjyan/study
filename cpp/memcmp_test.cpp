#include <cstring>
#include <cstdio>
#include <string>

struct Foo {
    int age;
    std::string name;
    const char *address;
};

int main() {
     int i = 10, j = 10;
     std::string str1 = "xiaoming", str2 = "xiaoming";
     const char *s1 = "hangzhou experimental school";
     const char *s2 = "hangzhou experimental school";

     Foo f1 = {i, str1, s1};
     Foo f2 = {j, str2, s2};


     printf("sizeof(Foo) is %u, %d\n", sizeof(Foo), memcmp(&f1, &f2, sizeof(Foo)) ); //conclusion: can not
}
