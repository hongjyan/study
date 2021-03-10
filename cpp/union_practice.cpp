#include <iostream>
#include <string>
using namespace std;

class Token {
private:
  enum {CHAR, INT, STR} tok;

  union {
    char cval;
    int ival;
    string sval;  //since union include a class member, so 
  };

  //gurantee copyunion to be call after class-type member be deconstructed/unintialized.
  void copyunion(const Token &t) {
    switch(t.tok) {
      case CHAR: cval = t.cval; break;
      case INT: ival = t.ival; break;
      case STR: new (&sval) string(t.sval); break; 
    }
  }

  void moveunion(Token &&t) {
    switch(t.tok) {
      case CHAR: cval = t.cval; break;
      case INT: ival = t.ival; break;
      case STR: 
        new (&sval) string(std::move(t.sval)); //shoud set t.sval to null?
        break;
    }
  }

public:
  friend ostream &operator<<(ostream&, Token&);
  //constructor
  Token(): tok(INT), ival(0) {} //annonymous union 's member can be access directly from outer class.
  Token(char c): tok(CHAR), cval(c) {}
  Token(int i): tok(INT), ival(i) {}
  Token(string s): tok(STR), sval(s) {}
  Token(const char* str): tok(STR), sval(str) {}

  //copy-constructore, move-constructor
  Token(const Token& t): tok(t.tok) { copyunion(t); cout << "copy-ctr" << endl; }
  
  Token(Token &&t) noexcept : tok(t.tok)  {
    cout << "I am move-ctr" << endl;
    moveunion(std::forward<Token>(t));
  }
  
  //operator=  
  Token &operator=(const Token &rhs) {
    if (this == &rhs) {
      return *this;
    }
   
    if (tok==STR && rhs.tok!=STR) { 
      sval.~string();
    }

    if (tok==STR && rhs.tok==STR) {
      sval = rhs.sval;
    }
    else {
      copyunion(rhs);
    }

    tok = rhs.tok;
    return *this;
  }

  Token &operator=(Token &&rhs) noexcept {
    cout << "I am move-operator=" << endl;
    if (tok == STR) sval.~string();
    moveunion(std::forward<Token>(rhs));
    tok = rhs.tok;
    return *this;
  }

  Token &operator=(char c) {
    if (tok == STR) sval.~string();
    cval = c;
    tok = CHAR;
    return *this;
  }

  Token &operator=(int i) {
    if (tok == STR) sval.~string();
    ival = i;
    tok = INT;
    return *this;
  }

  Token &operator=(string s) {
    if (tok == STR) 
      sval = s;
    else 
      new (&sval) string(s);

    tok = STR;
    return *this;
  }
  
  //de-constructor
  ~Token() {
    if (tok == STR) sval.~string();
  }


};


ostream &operator<<(ostream& os, Token& t) {
  switch(t.tok) {
    case Token::CHAR: os << "tok:CHAR, cval:" <<  t.cval << endl; break; 
    case Token::INT: os << "tok:INT, ival:" << t.ival << endl; break;
    case Token::STR: os << "tok:STR, sval:" << t.sval << endl; break;
  }
  return os;
}

int main() {
  Token a; 
  Token b = 'c'; 
  Token c = 1;
  Token d = "str";
  cout << a << b << c << d;

  a = b;
  c = d;
  cout << a << b << c << d;

  Token e = Token(122); cout << e; //compiler optimise, use constructor instead of move-costructor.
  e = Token("foo"); cout << e;
  
  Token f(e);
  return 0;
} 
