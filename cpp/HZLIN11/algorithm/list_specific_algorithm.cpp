#include <iostream>
#include <list>
#include <algorithm>
#include <iterator>
#include "../include/output.h"
using namespace std;


bool LT66(int val) {
    return (val < 66);
}

int main() {
    int a[] = {2, 3, 1, 4, 8, 9, 77, 65, 34};
    list<int> lst(a, a+9);
    output_lst(lst);


    list<int> lst2(10, 17);
    output_lst(lst2);
    
    lst.merge(lst2);
    output_lst(lst);
    output_lst(lst2);

    lst.reverse();
    output_lst(lst);

    lst.sort();
    output_lst(lst);
    
    list<int> lst3(9, 88);
    lst.splice(++lst.begin(), lst3); //entire list
    output_lst(lst);


    list<int> lst4(7, 188);
    lst.splice(lst.begin(), lst4, lst4.begin(), --lst4.end() );  //element range
    output_lst(lst);

    lst.splice(lst.end(), lst4, --lst4.end() );   //single element from other list
    output_lst(lst);

    lst.splice(lst.begin(), lst, --lst.end()); //single element from itself
    output_lst(lst);


    lst.remove(188);
    output_lst(lst);

    lst.remove_if(LT66);
    output_lst(lst);

    lst.unique();
    output_lst(lst);
    
}
