#include <stdio.h>
#include <exception>
int main() {
    try {
        throw "exception";
    }
    catch (...){
        printf("catch you\n");
        try {
            throw "next exception";
        }
        catch (const char* e) {
            printf("cath you again\n");
            throw "exception not in catch";
        }
        catch (...) {
            printf("won't come here\n");
        }
    }
}
        
