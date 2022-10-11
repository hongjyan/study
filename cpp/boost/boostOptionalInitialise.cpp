#include <boost/optional.hpp>
#include <string>
#include <cstdio>

struct st_vrf {
    std::string name;
    int age;
};

int main() {
    //st_vrf vrf = {"game", 10};
    //boost::optional<st_vrf> bo_vrf = vrf;
    boost::optional<st_vrf> bo_vrf = {"game", 10};
    printf("vrf.name is %s, vrf.age is %d\n", bo_vrf.get().name.c_str(), bo_vrf.get().age);

    //2022.11.4测试boost::none 与false之间的比较
    if (!boost::none) printf("should print\n");
    if (false == boost::none) print("should note print\n");
    return 0;
}
