#include <gecode/int.hh>
#include <gecode/set.hh>
#include <gecode/minimodel.hh>
#include <iostream>

class MySpace : public Gecode::Space
{
public:
 MySpace()
 : s(*this, 1, 9)
 , e(*this, 0, 9)
 , n(*this, 0, 9)
 , d(*this, 0, 9)
 , m(*this, 1, 9)
 , o(*this, 0, 9)
 , r(*this, 0, 9)
 , y(*this, 0, 9)
 {}

 MySpace(MySpace& other) : Gecode::Space(other)
 {
  s.update(*this, other.s);
  e.update(*this, other.e);
  n.update(*this, other.n);
  d.update(*this, other.d);
  m.update(*this, other.m);
  o.update(*this, other.o);
  r.update(*this, other.r);
  y.update(*this, other.y);
  
  Gecode::IntVarArgs allVariables;
  allVariables << s << e << n << d << m << o << r << y;
  Gecode::branch(*this, allVariables, Gecode::INT_VAR_SIZE_MIN(), Gecode::INT_VAL_SPLIT_MIN());
 }

 Gecode::Space* copy() override
 {
  return new MySpace(*this);
 }

 void print()
 {
  std::cout << "s: " << s << std::endl;
  std::cout << "e: " << e << std::endl;
  std::cout << "n: " << n << std::endl;
  std::cout << "d: " << d << std::endl;
  std::cout << "m: " << m << std::endl;
  std::cout << "o: " << o << std::endl;
  std::cout << "r: " << r << std::endl;
  std::cout << "y: " << y << std::endl;
 }

 Gecode::IntVar s, e, n, d, m, o, r, y;
};


int main()
{
 MySpace* home = new MySpace();
 home->print();


 Gecode::IntArgs multipliers4{1000, 100, 10, 1};
 Gecode::IntArgs multipliers5{10000, 1000, 100, 10, 1};

 Gecode::IntVarArgs sendVariables;
 sendVariables << home->s << home->e << home->n << home->d;
 Gecode::IntVar send(*home, 1000, 9999);
 Gecode::linear(*home, multipliers4, sendVariables, Gecode::IRT_EQ, send);

 Gecode::IntVarArgs moreVariables;
 moreVariables << home->m << home->o << home->r << home->e;
 Gecode::IntVar more(*home, 1000, 9999);
 Gecode::linear(*home, multipliers4, moreVariables, Gecode::IRT_EQ, more);

 Gecode::IntVarArgs moneyVariables;
 moneyVariables << home->m << home->o << home->n << home->e << home->y;
 Gecode::IntVar money(*home, 10000, 99999);
 Gecode::linear(*home, multipliers5, moneyVariables, Gecode::IRT_EQ, money);

 Gecode::rel(*home, (send + more) == money);

 home->status();
 home->print();

 Gecode::DFS<MySpace> search(home);

 while (MySpace* newSolution = search.next())
 {
  newSolution->print();
  delete newSolution;
 }

 return 0;
}