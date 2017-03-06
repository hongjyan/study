#include <iostream>
#include <stdio.h>
#include <string>
#include <cstdlib>
using namespace std;

int main() {
    char *str = NULL;
    char *str1 = (char *) malloc( 100 * sizeof(char) );
    printf("please input a string\n");
    if (scanf("%s", str) ) {       //can't scanf to a NULL pointer.
	printf("str is %s\n", str);
    }

    if ( scanf("%s", str1) ) {
	printf("str1 is %s\n", str1);
    }
    free(str1);

    string s;
    cout<<"please input a string"<<endl;
    cin>>s;
    cout<<"s is "<<s<<endl;
}
