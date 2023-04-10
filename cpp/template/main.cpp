
#include "Rule.hpp"
#include <cstdio>

int main()
{
    Rule<int> ir6(igt6);
    Rule<size_t> szr6(szgt6);
    Rule<size_t> sz_r6(ir6);
    printf("sz_r6(3) is %d\n", sz_r6(3));
    return 0;
}
