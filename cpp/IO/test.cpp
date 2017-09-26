#include <sstream>
#include <cstdio>

int main() {
    std::ostringstream oss;
    int i = 12345;
    oss << i;
    printf("%s\n", oss.str().c_str());
}
