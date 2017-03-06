#include "unp.h"

void Printf(const char *str) {
    printf("%s, ERRNO is %s\n", str, strerror(errno));
}
