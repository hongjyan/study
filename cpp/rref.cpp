#include <cstdio>
#include <iostream>
using namespace std;

class  Beta_ab {
public:
  Beta_ab(int sz, int val): size(sz) { 
    array = new int[size]{val};
    cout << "array is " << array << endl;
    cout << "Beta_ab ctr(int, int) " << this << endl;
  }

  Beta_ab(Beta_ab &&src) {
    cout << "Beat_ab move-ctr" << endl;
    free();
    array = src.array;
    size = src.size;
    src.array = nullptr;
    src.size = 0;
  }
  
  int* getArray() const { return array; }

  ~Beta_ab() { 
    cout << "Beta_ab de-ctr " << this << endl; 
    free();
  }

private:
  void free() { 
    if (nullptr != array) delete [] array; 
    size = 0; 
    array = nullptr;
  }

  int size{0};
  int *array{nullptr};
};


struct Beta {
  Beta_ab ab{10, 1};
  Beta_ab const& getAB() const& { 
    cout << "Beta_ab const& getAB() const&" << endl;
    return ab; 
  }

  // the ref-qualifier && says that "Beta_ab b3 = Beta().getAB();" is invoked on rvalue temporaries, making the following move, instead of copy
  Beta_ab && getAB() && { 
    cout << "Beta_ab && getAB() &&" << endl;
    return move(ab); 
  }
};



int main() {
  cout << "it's ok to bind a lvalue ref to rvalue ref" << endl;
  int &&rref = 10;
  int &ref = rref;
  ref = 12;
  printf("rref is %d, ref is %d, &rref is %p, &ref is %p\n", rref, ref, &rref, &ref);
  cout << endl << endl << endl;



 
  int *p = nullptr;
  {
    Beta beta;
    const Beta_ab &b1 = beta.getAB();
    p = b1.getArray();
  }
  cout << "p is " << p << endl;
  if (nullptr != p) {   //p won't be null. 
     cout << p[8] << endl;  //may lead to crash
     p[8] = 11;
     cout << p[8] << endl;
  }

  cout << "---------------------------" << endl;
  Beta_ab &&b2 = Beta().getAB();
  p = b2.getArray();    //p is null
  cout << "p is " << p << endl;
  if (nullptr != p) {
     cout << p[0] << endl;
  }

  cout << "---------------------------" << endl;
  Beta_ab b3 = Beta().getAB();   //use move-ctr
  p = b3.getArray();
  cout << "p is " << p << endl;
  if (nullptr != p) {
     cout << p[0] << endl;
  }
  
  cout << endl << endl << endl;



  return 0;
} 
