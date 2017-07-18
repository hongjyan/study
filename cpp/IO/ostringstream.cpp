#include <sstream>
#include <iostream>
#include <inttypes.h>
#include <cstdio>

using namespace std;

int main()
{
    uint8_t i = 35;
    ostringstream oss;
    oss << i;
    cout << oss.str() << endl;
    printf("i is %u\n", i);
    return 0;
} 
