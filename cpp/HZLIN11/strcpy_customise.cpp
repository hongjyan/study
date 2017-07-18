#include <iostream>
#include <assert.h>
#include <cstdlib>
#include <string.h>
using namespace std;
char* strcpy(char *strDst, const char *strSrc) {
    char *address = strDst;
    assert((strDst != NULL) && (strSrc != NULL));
    while((*strDst++ = *strSrc++) != '\0') {
	NULL;
    }
    return address;
}

int main() {
    char src[] = "ant and elephant";
    char *p = (char *) malloc(sizeof(char) * strlen(src) + 1);
    strcpy(p, src);
    cout<<p<<endl;
}
