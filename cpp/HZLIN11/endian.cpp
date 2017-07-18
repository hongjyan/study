#include <stdio.h>
#include <arpa/inet.h>
typedef unsigned char uint8_t;

typedef struct {
    uint8_t a:1;
    uint8_t b:1;
    uint8_t c:1;
    uint8_t d:1;
    uint8_t e:1;
    uint8_t f:1;
    uint8_t g:1;
    uint8_t h:1;
} stuc;

int readIntFromBuf(char** buf) {
    int ret = *((int *)*buf);
    *buf += sizeof(int);
    return ret;
}

int main(int argc, char* argv[]) {
    char c = 0x41;
    printf("Low bit--->High bit\n");
    for(int i=0; i<8; i++) {
	printf("%d: bit is %d\n", i, (c>>i) & 0x01);
    }
    
    stuc *st = (stuc *) &c;
    printf("st->a is %d\n", st->a);
    printf("st->b is %d\n", st->b);
    printf("st->c is %d\n", st->c);
    printf("st->d is %d\n", st->d);
    printf("st->e is %d\n", st->e);
    printf("st->f is %d\n", st->f);
    printf("st->g is %d\n", st->g);
    printf("st->h is %d\n", st->h);

    int i = 0x81124110;
    printf("Low byte---->High byte\n");
    for (int j=0; j<4; j++) {
	printf("%d: byte is 0x%x\n", j, (i>>j*8) & 0xff);
    }


    char *p = (char *) &i;
    printf("Low address--->High address\n");
    for (int j=0; j<4; j++) {
	printf("%x\n", *(p+j)); 
    }
//2016.5.11    
    char ch = 0x1c;
    int i_ch =  (int)ch;
    printf("i_ch is %x\n", i_ch);
    printf("htonl(i_ch) is %x\n", htonl(i_ch)); //reverse here(in Intel), do nothing in PowerPC since network order is the same as host order in PowerPC
    printf("ntohl(i_ch) is %x\n", ntohl(i_ch)); //same as above
    int n_h_ich = ntohl(htonl(i_ch));
    int h_n_ich = htonl(ntohl(i_ch));
    int n_n_ich = ntohl(ntohl(i_ch));
    int h_h_ich = htonl(htonl(i_ch));
    printf("n_h_ich is %x, h_n_ich is %x, n_n_ich is %x, h_h_ich is %x\n", n_h_ich, h_n_ich, n_n_ich, h_h_ich);
    char *pch = (char *)&i_ch;
    for (int i=0; i<4; i++) {
	printf("%x\n", *(pch+i));
    }

//2016.5.11
    struct st_ageWeight {
	int age;
	char c;
	int weight;
    };
    printf("sizeof(st_ageWeight) is %u\n", sizeof(st_ageWeight));
    st_ageWeight st_person = {21, 'c', 0x45};
    
    struct st_ageWeight *p_st_person = &st_person;
    char **pp_st_person = (char**)&p_st_person;

    int age = readIntFromBuf(pp_st_person);
    char m = **pp_st_person;
    char m_next = *(*pp_st_person+1);
    char m_next_next = *(*pp_st_person+2);
    (*pp_st_person)++;
    int weight = readIntFromBuf(pp_st_person);
    printf("age is %d, c is %c, m_next is %x, m_next_next is %x, weight is %x\n", age, m, m_next, m_next_next, weight);
}
