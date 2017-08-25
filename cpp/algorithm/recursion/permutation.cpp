#include <cstdio>
//#define DEBUG 1

inline void Swap(int &a,int &b)
{
        int temp=a;
        a=b;
        b=temp;
}
void Perm(int list[],int k,int m)
{
        if (k == m-1) 
        {
                for(int i=0;i<m;i++)
                {
                        printf("%d",list[i]);
                }
                printf("n");
        }
        else
        {
                for(int i=k;i<m;i++)
                {
                        Swap(list[k],list[i]); 
                        Perm(list,k+1,m);
                        Swap(list[k],list[i]); 
                }
        }
}

double factorial(int n) {
    if (1==n) {
        return 1;
    }
    return n*factorial(n-1);
} 


void swap(char* str, int i, int j)
{
    char temp = str[i];
    str[i] = str[j];
    str[j] = temp;
#ifdef DEBUG
    printf("swap index%d and index%d, string is %s after swap\n", i, j, str);
#endif
}

void permute(char *string, int start, int end)
{
#ifdef DEBUG
    printf("Enter permute, string is %s, start is %d, end is %d\n", string, start, end);
#endif
    if(start == end)
    {
        printf("%s\n", string);
        return;
    }

    permute(string, start + 1, end);
    int i;
    for(i = start + 1; i < end; i++)
    {
#ifdef DEBUG
        printf("Enter for loop, start is %d, i is %d\n", start, i);
#endif
        if(string[start] == string[i])
            continue;
        swap(string, start, i);
        permute(string, start + 1, end);
        swap(string, start, i);
    }
#ifdef DEBUG
    printf("Exit permute, string is %s, start is %d, end is %d\n", string, start, end);
#endif
} 
    
void permuteCSDN(char *string, int beg, int end) {
    if (beg == end) {
        printf("%s\n", string);
        return;
    } else {
         for(int i=beg; i<end; i++) {
             swap(string, beg, i);
             permuteCSDN(string, beg+1, end);
             swap(string, beg, i);
         }
    }
}

int main(int argc, char* argv[]) 
{
    int list[] = {1, 2, 3, 4};
    Perm(list, 2, 4);
    printf("*************************************\n");
    char str[] = {'1', '2', '3', '4'};
    permute(str, 0, 4);
    printf("*************************************\n");
    permuteCSDN(str, 0, 4);
    return 0;
}
