#include <cstdio>

bool ifIncreasing(int* array, int n) {
    if (array[0]<=array[1] && 2==n) {
        return true;
    }

    return (array[0]<=array[1] && ifIncreasing(array+1, n-1));
}

int main() {
     int arr1[] = {1, 2, 3, 4, 5, 6};
     int arr2[] = {1, 2, 7, 4, 10, 3};
     printf("%d\n", ifIncreasing(arr1, sizeof(arr1)/sizeof(int)));
     printf("%d\n", ifIncreasing(arr2, sizeof(arr2)/sizeof(int)));
     return 0;
}

