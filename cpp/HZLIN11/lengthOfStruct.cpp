#include <stdio.h>
#include "./lib/Util.cpp"
#ifndef byte
typedef unsigned char byte;
#endif
int main(int argc, char* argv[]) {
    struct rfc2833_pack_struct {
        byte event:8;
        byte volume:6;
        byte reserved:1;
        byte end_of_event:1;
        byte durationLow:8;
        byte durationHigh:8;
    };
    struct rfc2833_pack_struct pack;
    pack.event = 4;
    pack.volume = 0;
    pack.reserved = 0;
    pack.end_of_event = 1;
    pack.durationLow = 0x1F;
    pack.durationHigh = 0x40;

//    printf("size of rfc2833_pack_struct is %d\n", sizeof pack);
    printf("pack content is %s\n", toHexString((unsigned char*)&pack, sizeof pack).c_str()); 
}
    
