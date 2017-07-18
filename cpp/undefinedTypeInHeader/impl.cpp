#include "namespaceProxy.hpp"
#include <cstdio>

void showName(NamespaceProxy& nsp) {
    printf("nsp.name is %s\n", nsp.nsp_.c_str());
} 
