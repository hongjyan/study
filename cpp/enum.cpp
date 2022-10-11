#include <iostream>
#include <cstdio>


enum  NUM{
    one,
    two = 2,
    three,
    four = 14,
    five
};

//2022.10.12
void foo(bool b) {
    b ? std::cout << "True" : std::cout << "False";
}

int main() {
    NUM i = static_cast<NUM>(1);
    NUM j = static_cast<NUM>(10);  //won't crash or exception to assign non-exist value to enum type variable.
    printf("j is %d\n", j);
    std::cout << "i is " << i << ", j is " << j << std::endl;
    std::cout << "one is " << one << ", two is " << two << ", three is " << three  << 
    ", four is " << four << ", five is " << five << std::endl; 

    foo(one);
    foo(two);
    return 0;
}

