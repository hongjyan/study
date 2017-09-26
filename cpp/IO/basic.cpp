#include <iostream>
#include <string>
#include <cstring>
#include <assert.h>
#include <stdexcept>
#include <fstream>
using namespace std;


istream& read_write(istream &in, ostream &out) {
    assert(in && out);
    string s;
    cout<<"keep input something, 'quit' or 'QUIT' to exit"<<endl;
    while (in>>s) {
	if (in.eof()) break;
	
	if (strcmp(s.c_str(), "QUIT") == 0) break;
    
	if (s.compare("quit") == 0) break;
	
	if (in.bad() ) {
	    throw runtime_error("IO stream corrupted");
	}
	
	if (in.fail() ) {
	    cerr<<"bad data, try again"<<endl;
	    in.clear(istream::failbit);
	}
	
	out<<s<<endl;
    }
    
    in.clear();
    return in;
}
    

int main() {
    int ival;
    string str;
    cout<<"input a inter:";
    cin >> ival;
    cout<<"input a string:";
    cin >> str;
    cout<<"ival is "<<ival<<endl;
    cout<<"str is "<<str<<endl;

    read_write(cin, cout);

//write something into file
    string file_name = "basic.txt";
    ofstream f;
    f.open(file_name.c_str() );  //learn use c_str()
    f<<"anything here";
    f.close();

//copy content from one file to another file
    ifstream f_in("basic.txt"); //iostream 's constructor and member-function:open only accept c_str type para.
    ofstream f_out("basic_copy.txt");
    read_write(f_in, f_out);
    f_in.close();
    f_out.close();

//write something into file
    ofstream file_out("basic2.txt");
    read_write(cin, file_out);
    file_out.close();
} 
