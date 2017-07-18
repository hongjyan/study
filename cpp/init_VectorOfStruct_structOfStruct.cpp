#include <vector>
#include <string>
#include <cstdio>

struct ST1 {
    double a;
    double b;
    std::vector<std::string> vec;
};

struct ST2 {
   int i;
   ST1 st1;
};


int main() {
    ST1 st1 = {0.1, 2, {"fun", "foo"} };
    ST1 nexst1 = {0.1, 2, {} };
    printf("st1.vec[1] is %s\n", st1.vec[1].c_str());
    printf("The size of nexst1.vec is %lu\n", nexst1.vec.size());

    ST2 st2 = {1, {0.1, 2, {"fun", "hello"}} };
    printf("st2.st1.vec[1] is %s\n", st2.st1.vec[1].c_str());
    return 0;
}
