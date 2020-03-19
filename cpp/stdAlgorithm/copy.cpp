#include <cstdio>
#include <algorithm>
#include <vector>
#include <list>
using namespace std;

int main() {
    int a[] = {1, 2, 3, 4};
    int a2[sizeof(a)/sizeof(*a)];


//destination range should be big enough
    auto ret = copy(begin(a), end(a), a2);
    for (auto e: a2) {
         printf("%d\t", e);
    }
    printf("\n");
    printf("*ret is %d\n", *ret); //1, can not understand, the iterator after last element points to first element?
    printf("*ret is %d\n", *(--ret)); //4

//test front_inserter
    list<int> lst(begin(a), end(a));
    list<int> lst1, lst2;
    copy(lst.begin(), lst.end(), front_inserter(lst1)); //lst1 should be 4 3 2 1
    for (auto e: lst1) {
        printf("%d\t", e);
    }
    printf("\n");

    
    copy(lst.begin(), lst.end(), inserter(lst2, lst2.begin())); //lst2 should be 1 2 3 4
    for (auto e: lst2) {
        printf("%d\t", e);
    }
    printf("\n");

    return 0;
}
