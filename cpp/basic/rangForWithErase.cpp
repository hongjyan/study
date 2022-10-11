#include <set>
#include <cstdio>

int main()
{
    std::set<int> is{1};
    for (const auto& e : is)  {
        printf("e is %d", e);
        if (1 == e) is.erase(e);
    }
    printf("end");
}