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
    return 0;
}
