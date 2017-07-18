#include <string.h>
#include <stdio.h>

int main() {
    unsigned char arr[3] = {4, 64};
//    memset(arr, 4, 1);
//    memset(arr+1, 64, 1);
    printf("arr[0] is %d\n", arr[0]);
    printf("arr[1] is %d\n", arr[1]);
    printf("arr[2] is %d\n", arr[2]);
}
