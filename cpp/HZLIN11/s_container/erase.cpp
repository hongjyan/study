#include <iostream>
#include <vector>
using namespace std;

int main() {
    int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    vector<int> ivec(a, a+10);
    typedef vector<int>::iterator it;
    it p = ivec.begin() + 3;
    cout << *ivec.erase(p) << endl;
    if (ivec.end() == ivec.erase(ivec.begin(), ivec.end()) ) {
	cout << "end iterator" << endl;
    }

    vector<int> ivec2(a, a+10);
    ivec2.erase(ivec2.begin(), ivec2.end()-1); //ivec2.end()-1 won't be deleted.
    cout << ivec2.size() << endl;
}
