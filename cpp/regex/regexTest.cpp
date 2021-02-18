/*------------------------
https://stackoverflow.com/questions/15671536/why-does-this-c11-stdregex-example-throw-a-regex-error-exception
currently, std::regex support is incomplete in GCC 4.8, use boost::regex instead.
it was supported in GCC4.9.0
--------------------------*/
#include <iostream>
#include <string>
#include <regex>
using namespace std;


int main() {
  string pattern("[^c]ei");
  try {
    pattern = "[[:alpha:]]*" + pattern + "[[:alpha:]]*";
    regex r(pattern, std::regex::egrep|std::regex::extended);
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
