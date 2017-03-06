#include <unistd.h>

int main() {
    execl("getopt_test.o",  "--omited", "--apple", "--boy", "--cat=dahuang", "--dog=xiaohei", NULL);
    return 0;
}
