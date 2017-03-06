#include <iostream>
#include <list>
#include <limits>
using namespace std;

int main() {
    list<int> il(10,77);

    typedef list<int>::iterator it;
    it iter = il.begin();
    int temp;

    cout << "keep input integer, non-interger to stop"<<endl;
    while (cin >> temp) {
	iter = il.insert(iter, temp);
    }
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << cin.failbit << cin.badbit << endl;

    cout << "keep input integer, non-interger to stop"<<endl;
    while (cin >> temp) {
	il.push_front(temp);
    }
    
    iter = il.begin();
    while (iter != il.end()) {
	cout << *iter++ << endl;
    }
}
	
