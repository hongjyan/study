#include <stdio.h>

char *aDog = "roamPuffy";

void renameDog(char* pDog) {
    pDog[0] = 'I';
}

void tryReplace_butFail(char* pDog) {
    pDog = aDog;
}

void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

int main(int argc, char* argv[]) {
    char mydog[] = "Maxxxxxxxx";
    printf("%s is my dog\n", mydog);

    renameDog(mydog);
    printf("%s is my dog's new name\n", mydog);

    tryReplace_butFail(mydog);
    printf("%s is my dog still\n", mydog);

    int a = 3, b = 7;
    swap(a, b);
    printf("a is %d, b is %d\n", a, b);
    return 0;
}
    
