#include <stdio.h>
void foo(int value) {
    if (value > 9) {
	printf("value is bigger than 9\n");
        return; //if this line is not added, won't work as expect. But this is bad style, should use if...else
    }
    printf("value is not bigger than 9\n");
}

int main(int argc, char* argv[]) {
    foo(10);
    foo(9);
}
    
