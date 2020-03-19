#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;
int main()
{
    int array[] = {10,10,20,20,30,30,10};
    std::vector<int> ivec(array, array+sizeof(array)/sizeof(int));
    
    std::vector<int>::iterator it = std::unique(ivec.begin(), ivec.end());
    for (std::vector<int>::iterator beg=ivec.begin(); beg!=ivec.end(); beg++) {
        cout << *beg << " ";
    }
    cout << endl;

    ivec.resize(std::distance(ivec.begin(), it));
    it = std::unique(ivec.begin(), ivec.end());
    for (std::vector<int>::iterator beg=ivec.begin(); beg!=ivec.end(); beg++) {
        cout << *beg << " ";
    }
    cout << endl;

    for (std::vector<int>::iterator beg=it; beg!=ivec.end(); beg++) {
        cout << *beg << " ";
    }
    cout << endl;
    return 0;
}
