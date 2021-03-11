#include <iostream>

//nested class
int x, y;

class enclose {
  int x;
  enum { z, k};
  static int s;

  struct nestedPrivate {
    void g() {};
  };

public:
  struct nestedPublic {
    void f(int i) {
      //x = i; //Error: can't write to non-static enclose::x without instance. since 
      int a = sizeof x; // Error until C++11
                        // OK in C++11: operand of sizeof is unevaluated.
      int b = sizeof s;

      s = i;    // OK: can assign to the static enclose::s 
      std::cout << k << std::endl;    
      ::x = i;  // OK: can assign to global x
      y = i;    // OK: can assign to global y
    }


    void g(enclose *p, int i) {
      p->x = i;
    }

  private:  
    int m;
  };

  nestedPrivate f(nestedPublic *p) {
    //int j = p->m; //error: 'int enclose::nestedPublic::m' is private
    return nestedPrivate();
  }
};


//local class
void containing_f() {
  static int x;
  enum { i, j };
  int y;

  struct Local {
    void method() {
      std::cout << x << i << std::endl;
      //std::cout << y << std::endl;  //wrong, can not access non-static/non-enum variable of containing function
    }
  };

  Local l;
  l.method();
}


int main() {
  enclose e;
  enclose::nestedPublic e_public;

  //enclose::nestedPrivate e_private = e.f(&e_public); //error: nestedPrivate is private
  auto e_private = e.f(&e_public); //auto is ok! tricky!
  e_private.g(); //we can call private-class's public method via auto!

  containing_f();
  //Local l;  //local class can only be accessed by containing function.
  return 0;
}



