#include <cstdio>
void swap(int *array, int i, int j) {
    array[i] = array[i] + array[j];
    array[j] = array[i] - array[j];
    array[i] = array[i] - array[j];
}

void sawtoothWave(int *array, size_t size) {
    bool flip = true;
    for (size_t i=0; i<size; i++) {
        for (size_t j=i+1; j<size; j++) {
             if (flip) {
                 if (array[i] > array[j]) swap(array, i, j);
                 flip = !flip;
                 break;
             } else {
                 if (array[i] < array[j]) swap(array, i, j);
                 flip = !flip;
                 break;
             }
        }
     }
     
     for (size_t i=0; i<size; i++) {
         printf("%d ", array[i]);
     }
     printf("\n");
}


void sort(int a[], int len)
{
    if (1==len)
        return;
    sort(a, len-1);
    if (!(len%2)) {
       if (a[len-1] < a[len-2])
          swap(a, len-1, len-2);
    }
    else {
       if (a[len-1] > a[len-2])
          swap(a, len-1, len-2);
    }
}
 
int main() {
    int array[] = {5 ,12, 43, 2, 45, 75, 99329, 122, 4, 6};
    sawtoothWave(array, sizeof(array)/sizeof(int));
    sort(array, sizeof(array)/sizeof(int));
    for (size_t i=0; i<sizeof(array)/sizeof(int); i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    return 0;
}                 
