#include <stdio.h>

char *aDog = "roamPuffy";

void renameDog(char* pDog) {
    pDog[0] = 'I';
}

void tryReplace_butFail(char* pDog) {
    pDog = aDog;
}

int main(int argc, char* argv[]) {
    char mydog[] = "Maxxxxxxxx";
    printf("%s is my dog\n", mydog);

    renameDog(mydog);
    printf("%s is my dog's new name\n", mydog);

    tryReplace_butFail(mydog);
    printf("%s is my dog still\n", mydog);
    return 0;
}
    
