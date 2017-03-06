#include <string>
#include <cstdio>

struct st_info{
    std::string name;
    //int age;
};

void foo(const st_info & info) {
    printf("name:%s\n", info.name.c_str());
}

void bar(const st_info * pinfo) {
    printf("name:%s\n", pinfo->name.c_str());
}

int main() {
    struct st_info info = {"xiaoming"};
    foo(info);
    std::string s("xiaoli");
    foo(s);
    bar(&s);
    return 0;
}
