#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdio>
#include <cstdlib>

using namespace std;
char const seed[] = "ABCDabcd";

int main() {
  cout << "input a line" << endl;
  string s;
  cin >> s;
  string bs = "";
  
  for(size_t i=0; i!=s.size(); i++) {
    for (int bit_index=7; bit_index>=0; --bit_index) {
      int bit = ((s[i] >> bit_index) & 1);
      std::ostringstream oss;
      oss << bit;
      bs += oss.str();
    }
  }
 
  cout << bs << endl;

  size_t count = bs.size()/3;
  size_t mod = bs.size()%3;
  vector<int> vi;
 
  for(size_t i=0; i!=count; i++) {    
    string b3 = bs.substr(i*3, 3);
    vi.push_back(strtol(b3.c_str(), NULL, 2));
  }
 
  if (mod) {
    string bmod = bs.substr(count*3, mod);
    bmod += string(3-mod, '0');
    vi.push_back(strtol(bmod.c_str(), NULL, 2));
  }

  string encoded;    
  for(vector<int>::const_iterator it=vi.begin(); it!=vi.end(); it++) {
    cout << *it << " ";
    encoded += seed[*it];
  }
  cout << endl;
  cout << encoded << endl;
  

  return 0;
}
