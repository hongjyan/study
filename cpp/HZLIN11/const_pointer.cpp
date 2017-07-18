#include <stdio.h>
int main() {
    char* p0 = NULL;
    char array[] = "array";
    p0 = array;
    printf("p0 is %s\n", p0);
    *p0 = 'A';
    printf("p0 is %s in the second time\n", p0);

    const char *p1 = "game"; //const decorate char, means p1 pointer to const char type. 
    printf("p1 points to %s\n", p1);
//    *p1 = 'G';  //p1 points to const variable
    char const *p2 = "fun";  //recommend to use const char *p than char const *p.
    printf("p2 points to %s\n", p2);
//    *p2 = 'F';  //p2 points to const variable
    p1 = p2;
    printf("p1 points to %s\n", p1);    
    

    char* const p3 = array;  //const decorate p3, means p3 is a const pointer.
//    char* const p3 = "game";  //OK here. But segement fault at *p3 = 'L'; since p3 points to a "const memory where store const string:game".
    printf("p3 points to %s\n", p3);
    *p3 = 'L';
    printf("p3 points to %s in the second time\n", p3);
//    p3 = p2; //we can't change p3's value since it a const pointer.
    
    const char* const p4 = "I am a const pointer that points to a const variable";
    printf("p4 pointrs to %s\n", p4);
//    p4 = p3;  //can't change const pointer p4
//    *p4 = 'i'; //can't change the const variable that p4 points to
}
