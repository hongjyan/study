#include <iostream>
#include <map>
#include <string>
#include <typeinfo>
using namespace std;

int main() {
    typedef map<string, int>::iterator iter;
    typedef map<string, int>::value_type v_type;
    map<string, int> m;

    m["A"] = 1;
    m.insert(make_pair("B", 2));

    for (iter it=m.begin(); it != m.end(); ++it) {
	cout << "key:value is " << it->first << ":" << it->second <<endl;
    }
    cout << "----------------------------------" << endl;

    pair<iter, bool> rc = m.insert(make_pair("B", 22));
    if (!rc.second) {
	cout << "insert failed, since key already exist" << endl;
    }
    for (iter it=m.begin(); it != m.end(); ++it) {
	cout << "key:value is " << it->first << ":" << it->second <<endl;
    }
    cout << "----------------------------------" << endl;
    
    map<string, int> m2(m);

    map<string, int> m3(m.begin(), m.end());

    map<string, int> m4;
    m4["B"] = 222;
    m4["C"] = 3;
    m4["D"] = 4;
    m4["E"] = 5;
    m4.insert(m.begin(), m.end());
    for (iter it=m4.begin(); it != m4.end(); ++it) {
	cout << "key:value is " << it->first << ":" << it->second <<endl;
    }
    cout << "----------------------------------" << endl;

    iter it = m4.begin();
    it++; 
    it++;
    it++;
    it++;
    iter it_insert = m4.insert(it, v_type("D", 44));
    for (iter it=m4.begin(); it != m4.end(); ++it) {
	cout << "key:value is " << it->first << ":" << it->second <<endl;
    }
    cout << it_insert->first << endl;
    cout << "----------------------------------" << endl;
    
    map<string, int>::value_type v;
    cout << typeid(v).name() << endl;
    map<string, int>::key_type k;
    cout << typeid(k).name() << endl;
    map<string, int>::mapped_type m_v;
    cout << typeid(m_v).name() << endl;

}
    
