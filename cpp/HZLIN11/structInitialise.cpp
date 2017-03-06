#include <stdio.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>


struct Person {
    int age;
    std::string name;
    const char* nickname[3];
};

struct Item {
    char* name;
};

struct S {
    char* p;
};

struct IPAddress {
    int family;
    union {
        struct in_addr in;
        struct in6_addr in6;
    } address;
};

int main() {
    printf("sizeof Person is %lu\n", sizeof(Person));

    //Way1: correct
    //Person p1 = {10, "xiaoming", {"ming", "game", "pao"} };

    //wrong, seems nickname of Person is not a pointer, but memory block
    /*
    char* nn[3] = { "ming", "pao", "hai" };
    Person p1 = {10, "xiaoming", nn};
    */

    //Way2: correct
    Person p1 = {10, "xiaoming"};
    p1.nickname = {"ta", "pao", "mi"};

    Person p2(p1);
    printf("&p1 is %p, &p2 is %p, p1.age is %d, p2.age is %d, p1.nickname[0] is %s, p2.nickname[0] is %s\n", 
            &p1, &p2, p1.age, p2.age, p1.nickname[0], p2.nickname[0]);
    p1.age = 11;
    p1.nickname[0] = "fei";
    //change content of p1, p2 content won't be affected.
    printf("&p1 is %p, &p2 is %p, p1.age is %d, p2.age is %d, p1.nickname[0] is %s, p2.nickname[0] is %s\n", 
            &p1, &p2, p1.age, p2.age, p1.nickname[0], p2.nickname[0]);



//simple example to prove memory copy is used when initialise struct instance by another instance.
    printf("sizeof Item is %lu\n", sizeof(Item));
    int *p;
    printf("sizeof p is %lu\n", sizeof(p));

    char sa[] = "toothBrush";
    Item item1 = { sa };
    Item item2(item1);
    sa[0] = 'T';
    printf("item1.name is %s, item2.name is %s\n", item1.name, item2.name);
    item1.name = "foo";
    printf("item1.name is %s, item2.name is %s\n", item1.name, item2.name);
    Item item3 = { "toothBrush" };
    Item item4(item3);
    item4.name = "towel";
    printf("item3.name is %s, item4.name is %s\n", item3.name, item4.name);
    item3.name = "ToothBrush";
    printf("item3.name is %s, item4.name is %s\n", item3.name, item4.name);

    struct S s1;
    s1.p = (char*) malloc(10);
    struct S s2(s1);
    memset(s1.p, 'a', 10);
    int i = 0;
    while (i<10)
    {
         printf("%c\n", *((s2.p)+i));
         i++;
    }
    free(s1.p);

//real example in_addr and in_addr6
    struct in_addr addr;
    if (0 == inet_aton("127.0.0.1", &addr)) { printf("wrong\n"); } 
    struct IPAddress ipv4;
    ipv4.family = AF_INET;
    ipv4.address.in = addr;
    struct in_addr addr_copy(addr);
    printf("%s\n", inet_ntoa(addr_copy));
    struct IPAddress ipv4_copy(ipv4);
    printf("%s\n", inet_ntoa(ipv4_copy.address.in));

    struct in6_addr addr6;
    if (1 != inet_pton(AF_INET6, "1:2:3:4:5:6:7:8", &addr6)) { printf("wrong\n"); }
    struct IPAddress ipv6;
    ipv6.family = AF_INET6;
    ipv6.address.in6 = addr6;
    struct in6_addr addr6_copy(addr6);
    char str[INET6_ADDRSTRLEN];
    printf("%s\n", inet_ntop(AF_INET6, &addr6_copy, str, INET6_ADDRSTRLEN));
    struct IPAddress ipv6_copy(ipv6);
    printf("%s\n", inet_ntop(AF_INET6, &(ipv6_copy.address.in6), str, INET6_ADDRSTRLEN));
    
}
