#include <cstdio>
int getRepeatedCharacter(const char* str) {
    static int num = 0;
    if (0==*str) return num;
    
    if (*str==*(str+1)) num++;

    getRepeatedCharacter(++str);
    return num;
}

int getMatchedNumber(const char* str1, const char* str2) {
    int  ret = 0;
    if (0==*str2) return ret;

    const char *p1 = str1;
    const char *p2 = str2;
    while(*p1++ == *p2++) {
        ret++;
    }

    int next1 = getMatchedNumber(str1, str2+1);
    if(next1 > ret) ret = next1;
    
    return ret;
}
    

int getRepeatedStringNumber(const char* str1, const char* str2) {
    int ret = 0; int current = 0;
    while(*str1) {
        current = getMatchedNumber(str1++, str2);
        if(current > ret) ret = current;
    }
    return ret;
}
    

int main() {
    printf("%d\n", getRepeatedCharacter("iiiiabcdefgghhijkfffn"));
    printf("%d\n", getRepeatedStringNumber("abc", "dfcabckljab"));
    printf("%d\n", getRepeatedStringNumber("abc", "dfc"));
    printf("%d\n", getRepeatedStringNumber("abcd", "kbcabe"));
    return 0;
}
