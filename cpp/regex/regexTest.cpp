/*------------------------
https://stackoverflow.com/questions/15671536/why-does-this-c11-stdregex-example-throw-a-regex-error-exception
currently, std::regex is not implemeted IN GCC, so use boost::regex instead.
--------------------------*/
#include <iostream>
#include <string>
#include <regex>
using namespace std;


int main() {
  string pattern("[^c]ei");
  try {
    pattern = "[[:alpha:]]*" + pattern + "[[:alpha:]]*";
    regex r(pattern, regex_constants::extended);
    smatch results;

    string test_str = "receipt freind theif receive";
 
    if (regex_search(test_str, results, r)) {
      cout << results.str() << endl;
    }
  }
  catch (regex_error &e) {
    cout << e.what() << endl << e.code() << endl;
  }
  
  return 0;
}
