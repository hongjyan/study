//2022.2.21
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

void cpy_file(string filename) {
  ifstream src(filename, std::ios::binary);
  ofstream dst(filename + ".cpy", std::ios::binary);
  dst << src.rdbuf();
}


void append_line_number(const string filename) {
  ifstream in(filename, fstream::in);
  if (!in) { 
    cout << "file:" << filename << " doesn't exsit." << endl;
    return; 
  }
  in.close();

  cpy_file(filename);
  fstream inout(filename + ".cpy", fstream::ate | fstream::in | fstream::out);

  auto endMark = inout.tellg();
  auto curMark = endMark;
  string line;
  size_t cnt = 0;
  cout << inout.fail() << inout.bad() << endl;

  inout.seekg(0, fstream::beg);
  while (std::getline(inout, line) && inout.tellg() != endMark) { //inout.tellg!=endMark is necessary, otherwise inout's eofbit will be set, which will cause succeeding operation on inout failed.
    //get line cnt and reserve current position
    curMark = inout.tellg();
    cnt = line.size() + 1; //1 for '\n'
    
    //go to end then write cnt and space
    inout.seekp(0, fstream::end);
    //inout.write(cnt);  //can not write int as char!
    inout << cnt;
    if (curMark != endMark) {
      inout << ' ';
    }
    
    //go back to reserved position
    inout.seekg(curMark);
  }
  
  //append newline
  inout.seekp(0, fstream::end);
  inout << "\n";

  //close file
  inout.close();
}
  

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


  /****fstream.seek/tell******/
  string filename = "test.txt";
  //ofstream fs;
  fstream fs;
  fs.open(filename);
  fs.write("this is an apple", 16);
  long pos = fs.tellp();
  fs.seekp(pos-7);
  fs.write(" sam", 4);
  fs.flush();
  string s;
  //ifstream infile(filename);
  fs.seekg(0);
  while (std::getline(fs, s)) {
    cout << s << endl;
  }
  fs.close();

  /*****fstream seek/tell again from book*********/
  cout << "input file name" << endl;
  cin >> filename;
  append_line_number(filename);
  return 0;
}
