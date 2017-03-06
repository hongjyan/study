#include <cstdio>
#include <cstring>

int main() {
    char *pnull = NULL;
    printf("sizeof pnull is %d\n", sizeof(pnull));
    char game[100];
    strcpy(game, pnull);
    printf("strlen(pnull) is %d\n", strlen(pnull));
    printf("pnull is %s\n", pnull);
    
    return 0;
}
