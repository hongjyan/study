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
    return 0;
}

