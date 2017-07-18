#include <iostream>
using namespace std;

int main() {
    for (int i=0; i<10; i++) {
        cout << i << endl;
        if (i==6) continue;
        i++;
    }

    int i = 0;
    while (i<10) {
        cout << i << endl;
        if (6 == i) continue;
        i++;
    }

    return 0;
}
