#include <iostream>
#include <fstream>
using namespace std;

int main() {
    ofstream ofs("stream_ways.txt");
    ofs<<"writing something into file\n"<<endl;
    ofs.close();

//check if conetent of fill still exist after writing it in app way
    ofs.open("stream_ways.txt", ofstream::app);
    ofs.close(); 

    ifstream ifs("stream_ways.txt");
    string s;
    while (ifs >> s) {
	cout << s;
    }
    cout<<endl;
    
    ifs.close();
    ifs.open("stream_ways.txt"); //close then open to move to the beginning of file
    while (getline(ifs, s) ) {  //getline will terminate when encounting new line, so new line won't print.
	cout << s;
    }
    

//check if content of file will be wiped after writing it via ate
    ofs.open("stream_ways.txt", ofstream::ate);
    ofs.close();

    ifs.close();
    ifs.open("stream_ways.txt"); //close then open to move to the beginning of file
    while (ifs >> s ) {
	cout<<s;
    }

//close file
    ifs.close();
}
