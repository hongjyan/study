#include <stdlib.h> //itoa, atoi
#include <cstdio>
#include <stdio.h>
#include <string> //std::string, std::stoi
#include <sstream> //std::stringstream
#include <iostream> //std::cout

int main() {
    printf("----------------------------str to int -------------------------------------------\n");
    printf("strtol(\"12b\", NULL, 10) is %ld\n", strtol("12b", NULL, 10)); //12, first character muset be valid(space, +/-, 0xwhenbase-16), stop at first invalid character, which is b here. 
    printf("strtol(\"a0\", NULL, 10) is %ld\n", strtol("a0", NULL, 10)); //0. return 0 if str is invalid.
    printf("strtol(\"12.23\", NULL, 10) is %ld\n", strtol("12.23", NULL, 10));
    printf("strtol(\"0.23\", NULL, 10) is %ld\n", strtol("0.23", NULL, 10));
    printf("strtol(\"-1\", NULL, 10) is %ld\n", strtol("-1", NULL, 10));
    printf("strtol(\"10\", NULL, 16) is %ld\n", strtol("10", NULL, 16)); //16. of course, can specify base explicitly
    printf("strtol(\"a0\", NULL, 0) is %ld\n", strtol("a0", NULL, 0)); //0, if base = 0, then base-10 if str not starting with 0x or 0
    printf("strtol(\"0xa0\", NULL, 0) is %ld\n", strtol("0xa0", NULL, 0)); //160, prefix:0x implies base-16
    printf("strtol(\"080\", NULL, 0) is %ld\n", strtol("070", NULL, 0)); //64. prefix:0 implies base-8.

//atoi is the same as strtol(str, NULL, 10)
    printf("atoi(\"12b\") is %d\n", atoi("12b"));
    printf("atoi(\"a0\") is %d\n", atoi("a0"));
    printf("atoi(\"12.23\") is %d\n", atoi("12.23"));
    printf("atoi(\"0.23\") is %d\n", atoi("0.23"));
    printf("atoi(\"-1\") is %d\n", atoi("-1"));

//std:stoi(str, p=NULL, base=10), introduce in C++11
    printf("std::stoi(\"12b\", NULL, 10) is %d\n", std::stoi("12b", NULL, 10)); 
    //printf("std::stoi(\"a0\", NULL, 10) is %d\n", std::stoi("a0", NULL, 10)); //throwing an instance of 'std::invalid_argument' 
    printf("std::stoi(\"12.23\", NULL, 10) is %d\n", std::stoi("12.23", NULL, 10)); 
    printf("std::stoi(\"10\", NULL, 16) is %d\n", std::stoi("10", NULL, 16));
    printf("std::stoi(\"0xa0\", NULL, 0) is %d\n", std::stoi("0xa0", NULL, 0)); 


    printf("----------------------------int to str -------------------------------------------\n");
    char buffer[80] = {0};
    //printf("itoa(12, buffer, 10) is %s\n", itoa(12, buffer, 10)); //itoa is not defined in ANSI-C and is not part of C++, so do not use it.
    std::cout << "std::to_string(12) is " << std::to_string(12) << std::endl; // 
    std::cout << "std::to_string(3.14) is " << std::to_string(3.14) << std::endl; // 
    std::cout << "std::to_string(0x10) is " << std::to_string(0x10) << std::endl; // 
    printf("----------------------------stringstream -------------------------------------------\n");
//stringstream
    std::stringstream ss;
    ss << 10;
    printf("ss.str() is %s\n", ss.str().c_str());
    int a;
    ss >> a;
    printf("a is %d\n", a);
    return 0;
} 
