//2022.2.21
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

int main() {
/********************byte unfromat operation*******************/
  istringstream iss("heloworld");

  //is.get(ch), os.put(ch)
  char ch;
  iss.get(ch);
  cout << ch << endl;
  cout.put(ch); cout << endl;
 
  //is.get()
  cout << char(iss.get()) << endl; //e

  char ch2 = char(iss.get()); //l
  cout << ch2 << endl;

  //is.putback()
  iss.putback(ch2);
  cout << char(iss.get()) << endl; //still l instead of o, since we put l back

  //is.unget()
  iss.unget(); //let is move back. so next get still is l other than o
  cout << char(iss.get()) << endl; 

  //is.peek()
  cout << char(iss.peek()) << endl; //o. return next byte from stream, but doesn't delete it from stream. return this byte as int.
  cout << char(iss.get()) << endl; //still be o since byte not deleted from stream by peek.



/**************************multibyte unformat operation*********************/
  istringstream iss2("heloworld\ndoSomethingAlwaysAndInPeace\nInsepectYouself\nmy baby");
  char sink[65535];
  
  /*********is.get(sink, size, delimter)***********/
  iss2.get(sink, sizeof(sink), '\n');
  cout << sink << endl;    //heloworld

  //iss2.get(sink, sizeof(sink), '\n'); //get nothing since delimeter '\n' still in istream. this line will lead side effect: stream failbit set. which will lead succeeding operation useless.  
  //cout << sink << endl;

  cout << iss2.fail() << endl;
  cout << iss2.bad() << endl;
  cout << iss2.tellg() << endl;
  iss2.ignore(1, '\n');  // or iss2.get();
  iss2.get(sink, sizeof(sink), '\n');
  cout << "read " << iss2.gcount() << " bytes." << endl;
  cout << sink << endl;
  
  /******is.getline(sink, size, delim) *******/
  iss2.get();
  iss2.getline(sink, sizeof(sink)); 
  cout << sink << endl;
  iss2.getline(sink, sizeof(sink));//getline consume then discard delimeter, so no need to ignore/get delimeter.
  cout << sink << endl;


  /***is.read(sink, size) os.write(source, size) ***/
  istringstream iss3("hipo jump\ncat rabbit\nwho is best?");
  iss3.seekg(0, ios::end);
  unsigned int len = iss3.tellg();
  iss3.seekg(0);
  iss3.read(sink, len);
  cout.write(sink, iss3.gcount()); 
  cout << endl;


  /****ostream.seekp/tellp******/
  string filename = "test.txt";
  ofstream outfile;
  outfile.open(filename);
  outfile.write("this is an apple", 16);
  long pos = outfile.tellp();
  outfile.seekp(pos-7);
  outfile.write(" sam", 4);
  outfile.close();
  string s;
  ifstream infile(filename);
  while (std::getline(infile, s)) {
    cout << s << endl;
  }
  infile.close();

  return 0;
}
