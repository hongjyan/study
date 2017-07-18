#include <iostream>
#include <string>
using namespace std;

int main() {
    string s = "hello world!!!!";
    const char *str = "my honor";
    string s2 = "hi, buddy";

//#############append##############
    s.append(4, '-');
    cout << s << endl;

    s.append(s2.begin()+4, s2.end() );
    cout << s << endl;

    s.append(s2);
    cout << s << endl;

//    s.append(s2, s2.size()-6); //wrong
    s.append(s2, s2.size()-6, 6);
    cout << s << endl;

    s.append(str);
    cout << s << endl;

    s.append(str, 2);
    cout << s << endl;
//##################replace###################
    s = "hello world!!!!";
    s2 = "hey";
    str = "hi";

//start s.replace(pos, n, arg);
    s.replace(s.size()-4, 4, 5, '>');
    cout << s << endl;

    //    s.replace(s.size()-5, 5, s.end()-4, s.end() );  //can't use s.replace(pos, n, b, e);

    s.replace(0, 5, s2); 
    cout << s << endl;
    
    s.replace(0, 3, s2, 0, 2);
    cout << s << endl;

    //    s.replace(0, 3, s2, 2); // no arg: (s, pos)

    s.replace(0, 2, str);
    cout << s << endl;

    s.replace(0, 2, str, 1);
    cout << s << endl;

//start s.replace(b, e, arg)
    s.replace(s.end()-5, s.end(), 4, '!');
    cout << s << endl;

    s.replace(s.begin(), s.begin()+1, s2.begin(), s2.end() );
    cout << s << endl;

    s.replace(s.begin(), s.begin()+2, s2);
    cout << s << endl;

//    s.replace(s.begin(), s.begin()+4, s2, 0, 3); //can use s.replace(b, e, s2, pos, n);

    s.replace(s.begin(), s.begin()+4, str);
    cout << s << endl;

    s.replace(s.begin(), s.begin()+2, str, 1);
    cout << s << endl;

}
