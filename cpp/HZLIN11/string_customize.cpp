#include <iostream>
#include <string.h>
using namespace std;

class String {
public:
    String(const char *str = NULL);
    String(const String &S);
    String& operator=(const String &other);
    ~String();
    void output();
private:
    char *m_data;
};

String::String(const char *str) {
   if(NULL == str) {
	m_data = new char('\0');
    }
    else {
	int len = strlen(str) + 1;
	m_data = new char[len];
	strcpy(m_data, str); 
    }
}

String::String(const String &s) {
    int len = strlen(s.m_data) + 1;
    m_data = new char[len];
    strcpy(m_data, s.m_data);
}

String& String::operator=(const String &other) {
    size_t len = strlen(other.m_data) + 1;
    if(this == &other) return *this;
    delete [] m_data;
    m_data = new char[len];
    strcpy(m_data, other.m_data);
    return *this;
}

String::~String() {
    delete [] m_data;
}

void String::output() {
    cout<<m_data<<endl;
}

int main() {
    String s1;  //use normal constuctor with default parameter
    s1.output();
    String s2("lift her up"); //use normal constructor with specific parameter
    String s3 = s2;  //use assignment function
    String s4(s2); //use copy constructor
    s2.output();
    s3.output();
    s4.output();
}
