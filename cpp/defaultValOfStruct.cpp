#include <cstdio>
#include <string>

struct st_vrf {
    std::string name;
    unsigned int id = 0;
};

int main() {
    st_vrf vrf;
    printf("vrf.name is %s, vrf.id is %d\n", vrf.name.c_str(), vrf.id);
    return 0;
}
