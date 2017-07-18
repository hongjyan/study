#include <cstdio>
#include <iostream>
#include <stdio>
#include <string>
typedef char BYTE;



int main() {
    printf("long size is %ld\n", sizeof(long));
    printf("long size is %ld\n", sizeof(int));
    int a = 0x01020304;
    printf("a is %x\n", a);
    char c = a;
    printf("c is %x\n", c);

    char bytes[sizeof a];
    std::copy(static_cast<const char*>(static_cast<void*>(&a)), 
              static_cast<const char*>(static_cast<void*>(&a)) + sizeof a, 
              bytes);
/*    std::copy(static_cast<const byte*>(static_cast<byte*>(&a)), 
              static_cast<const byte*>(static_cast<byte*>(&a)) + sizeof a, 
              bytes);*/
    for (int i=0; i<sizeof a; i++) { 
        printf("%x ", bytes[i]);
    }
    printf("\n");
    
    BYTE by = 'c';
    printf("%c\n", by);

/*
    long num = 6666;
    char str[64];
    int n = sprintf(str, "%ld", num);
    fflush(stdout);
    printf("%s is a string %d chars long\n", str, n);
*/

/*
    std::string s1 = "game";
    std::string s2 = "fun";
    char *str = s1.c_str();
    str = s2.c_str();
*/
    
}
    
