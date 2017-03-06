#include <iostream>
#include <iterator>
#include <fstream>
using namespace std;

int main() {
//story content:
//there was a temple long long ago
//there were a old monk and a little monk in this temple
    ifstream ifile("story.txt");
    if (!ifile) {
	cout << "Open file failed" << endl;
	return -1;
    }


    ofstream ofile("story_copy.txt");
    if (!ofile) {
	cout << "Open file to write failed " << endl;
	return -1;
    }

    istream_iterator<string> in(ifile);
    istream_iterator<string> in2(ifile);
    istream_iterator<string> eof;
    istream_iterator<string> eof2;
    ostream_iterator<string> out(ofile);
    string dump;


//use istream_iterator to read stuff from istream
//normal way to read stuff from istream.
    cout << *in << endl;  //there
    cout << *in2 << endl;  //was
    cout << *++in << endl; //a

    ifile >> dump;   //temple
    cout << dump << endl;  //
    cout << *in << endl; //a
    cout << *++in << endl;  //long
    
//    *in == "test"; //wrong. restrict: can't write into object pointed by istream_iterator

//non-member function: ==, !=;
    cout << "*in2:" << *in2 << endl; //was
    cout << " *in:" << *in << endl; //long
    cout << (in == in2) << endl;  //1
    cout << (eof == eof2) << endl;  //1
    
    
// use ostream_iterator = xxx or *ostream_iterator = xxx, to write stuff into bonding ostream. 
// "ostream_iterator = xx" or "ostream << xxx" will increase iterator point to ostream
    out = "there";
    out = " was";
    
// normal way to write stuff into ostream
    ofile << " a";
    out = " temple";
    ofile <<  endl;  //the content of file is "there was a temple".

    ifile.close();
    ofile.close();
}
