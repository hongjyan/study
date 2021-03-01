#include <cstdio>

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


typedef struct {
    char c1;
    char c2;
    char c3;
    char c4;
} stuc2;

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
    printf("\n\n\n");

    unsigned int num = 0x81124110;
    printf("for 0x%x\n", num);
    for (int i=0; i!=32; ++i) {
      printf("%d:bit is %d\n", i, (num>>i) & 0x01);
    }
    stuc2 *st2 = (stuc2 *) &num;
    printf("num->c1 is 0x%x\n", st2->c1);
    printf("num->c2 is 0x%x\n", st2->c2);
    printf("num->c3 is 0x%x\n", st2->c3);
    printf("num->c4 is 0x%x\n", st2->c4);
    printf("\n\n\n");
    return 0;
}

