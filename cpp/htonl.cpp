#include <cstdio>
#include <arpa/inet.h>

int main() { 
   char ch = 0x1c;
    int i_ch =  (int)ch;
    printf("i_ch is %x\n", i_ch);
    printf("htonl(i_ch) is %x\n", htonl(i_ch)); //reverse here(in Intel), do nothing in PowerPC since network order is the same as host order in PowerPC
    printf("ntohl(i_ch) is %x\n", ntohl(i_ch)); //same as above
    //reverse i_ch then reverse back, so still i_ch.
    int n_h_ich = ntohl(htonl(i_ch));
    int h_n_ich = htonl(ntohl(i_ch));
    int n_n_ich = ntohl(ntohl(i_ch));
    int h_h_ich = htonl(htonl(i_ch));
    printf("n_h_ich is %x, h_n_ich is %x, n_n_ich is %x, h_h_ich is %x\n", n_h_ich, h_n_ich, n_n_ich, h_h_ich);
    int n_m1c = ntohl(0x1c000000);
    int h_m1c = htonl(0x1c000000);
    int n_l1c = ntohl(0x0000001c);
    int h_l1c = htonl(0x0000001c);
    printf("n_m1c is %x, h_m1c is %x, n_l1c is %x, h_l1c is %x\n", n_m1c, h_m1c, n_l1c, h_l1c);
    return 0;
}
