#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<string> vs = {"abc", "efg", "hij" };

void printByLambda(ostream &os, vector<string> &words, char c) {
  for_each(words.begin(), words.end(), [&os, c] (const string &s) { os << s << c; });
  cout << endl;
}

ostream &print(ostream &os, const string &s, char c) {
  return os << s << c;
}

void printByBind(ostream &os, vector<string> &words, char c) {
  for_each(words.begin(), words.end(), std::bind(print, ref(os), std::placeholders::_1, c));
  cout << endl;
}

int main() {
  printByLambda(cout, vs, ' ');
  printByBind(cout, vs, ' ');
  return 0;
}

