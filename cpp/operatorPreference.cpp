#include <cstdio>


int main() {
    //pre++ has same priority with *, and left preference
    {
       int arr[] = {10, 20};
       int *p = arr;
       printf("arr[0] = %d, arr[1] = %d, *++p = %d\n", arr[0], arr[1], *++p); //10, 20, 20. before++ take effect at its own line.
       printf("*p = %d\n", *p); //20
    } 
    {
       int arr[] = {10, 20};
       int *p = arr;
       printf("arr[0] = %d, arr[1] = %d, ++*p = %d\n", arr[0], arr[1], ++*p); //11, 20, 11. 
       printf("arr[0] = %d, ++*p = %d, arr[0] = %d, arr[1] = %d\n", arr[0],  ++*p, arr[0], arr[1]); //12, 12, 11, 20. seems printf execute from the rightest  
    } 

//later++ has higher priority than */before++    
    {
       int arr[] = {10, 20};
       int *p = arr;
       printf("arr[0] = %d, arr[1] = %d, *p++ = %d\n",  arr[0], arr[1], *p++); //10, 20, 10. p was not incresed at its own line even after++ has higher priority than *. From my previous understanding, p should be increased when do dereference since ++ was done before dereference, but the fact is the moment when p is increased at finish moment of the eveluation of expression including after++.  
       printf("*p = %d\n", *p); //20. p is increased after the line including after++.
    } 

    return 0;
}

