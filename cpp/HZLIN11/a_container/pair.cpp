#include <iostream>
#include <utility>
#include <string>
using namespace std;

pair<string, int> g_P;
int main() {
    pair<string, int> p;  //use value-intialized the same as creating null vector. not the same as initiaze \
			    ordinary variable.
    cout << p.first << " " << p.second << endl;
    cout << "please input value for p.first and p.second" << endl;
    cin >> p.first >> p.second;
    cout << p.first << " " << p.second << endl; //we can operate p.first&p.second member directly since they \
						  are public.

    cout << g_P.first << " " << g_P.second << endl;

    pair<string, int> p2("game", 10);

    pair<string, int> p3 = make_pair("fun", 9); 

    pair<string, int> p4 = p2;
    cout << (p4==p2) << endl;

    
}
