#include <cstdio>

void sequence(int a[], int len, int sum, int base)
{
    int i, j;
    
    if (len < 1) return;
 
    i = 0;
    j = len-1;

    if (base) printf("%d ", base);

    if (sum == a[j]) {
        printf("%d\n", a[j]);
        sequence(a, len-1, sum, 0);
        return;
    }

    while (i<j) {
        if (a[i]+a[j] == sum) {
            printf("%d %d\n", a[i], a[j]);
            sequence(&a[i+1], len-2, a[j], a[i]);
            i++;
            j--;
        }
        else if (a[i]+a[j] > sum)
            j--;
        else
            i++;
    }
}

int main()
{
    int a[1000];
    for (int i=0; i<1000; i++)
        a[i] = i+1;
    sequence(a, 1000, 1000, 0);
    return 0;
}
