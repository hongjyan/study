#include <cstdio>
#include <cstring>
#include <algorithm>

bool delimeterFound(const char* str) {
    return !strcasecmp(str, "-nodename");
}

void printRange(int argc, char** argv)
{
    static int i = 1;
    printf("%d-----------\n", i);
    ++i;
    int j = 0;
    while (j!=argc) {
        printf("%s ", argv[j]);
        ++j;
    }
    printf("\n");
}


void parseParameter(int ac, char* av[])
{
    char **pre = av, **cur = av, **end = av+ac;
    printf("*end is %s\n", *end);

    cur = std::find_if(cur, end, delimeterFound);
    pre = cur;
    while (end != cur)  {
        pre = cur;
        cur = std::find_if(cur+1, end, delimeterFound);
        printRange(cur-pre, pre);
    }
}

int main(int argc, char* argv[])
{
    parseParameter(argc, argv);
    return 0;
}
