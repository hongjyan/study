#include <iostream>
#include <iomanip>
using namespace std;

int main(int argc, char **argv) {
  //cout manipulator
  cout << "default bool value: " << true << " " << false
       << "\nalpha bool value: " << boolalpha << true << " " << false << endl;
  
  cout << showbase;
  cout << "default: " << 20 << " " << 1024 << endl;
  cout << "octal:" << oct << 20 << " " << 1024 << endl;
  cout << "octal still: " << 20 << " " << 1024 << endl;
  cout << "hex:" << hex << 20 << " " << 1024 << endl;
  cout << "decimal:" << dec << 20 << " " << 1024 << endl;

  cout << noshowbase;
  cout << "hex:" << hex << 20 << " " << 1024 << endl;

  cout << showbase << uppercase << hex
       << "print in upper hex:" << 20 << " " << 1024 
       << noshowbase << nouppercase << dec << endl;

  int i = -16;
  double d = 3.14159;
  
  cout << left;
  cout << "i:" << "  " << setw(12) << i << "  " << "next col" << endl;  //setw won't change IO state as endl;
  cout << "d:" << "  " << setw(12) << d << "  " << "next col" << endl;
  cout << right; //restore to default right
  cout << endl << endl << endl;


  //cin manipulator
  char ch;
  while (cin >> ch) {  //space will be omitted in default.
    cout << ch;
  }
  cin.clear();

  cout << "again, keep space";
  cin >> noskipws;
  while (cin >> ch) {
    cout << ch;
  }
  cin >> skipws; //restore default
  cin.clear();
  cout << endl << endl << endl;

  return 0;
}
