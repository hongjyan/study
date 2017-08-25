#include <cstdio>
#include <cstring>

bool ifRepeated(const char** strArray, unsigned int len) {
    if (1==len) return false;

    bool ret = false;
    for(unsigned int i=1; i<len; i++) {
        if(0 == strcmp(strArray[0], strArray[i])) {
            return true;
        }
    }

    ret |= ifRepeated(strArray+1, len-1);
    return ret;
}

int main() {
    const char* strArray[] = {"game", "fun", "aa", "bb", "11", "22", "22"};
    printf("%d\n", ifRepeated(strArray, sizeof(strArray)/sizeof(const char*)));
    const char* strArray2[] = {"game", "fun", "aa", "bb", "11", "22"};
    printf("%d\n", ifRepeated(strArray2, sizeof(strArray2)/sizeof(const char*)));
    const char* strArray3[] = {"game", "fun", "fun", "aa", "bb", "11", "22"};
    printf("%d\n", ifRepeated(strArray3, sizeof(strArray3)/sizeof(const char*)));
    return 0;
}
