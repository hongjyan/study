#include <iostream>
#include <random>
#include <map>

using namespace std;

int main() {
  default_random_engine e;
  cout << e() << endl;
  uniform_int_distribution<unsigned> u(0, 9);
  for (size_t i=0; i!=20; ++i) {
    cout << u(e) << " ";
  }
  cout << endl << endl << endl;



  //seed
  default_random_engine e1;
  default_random_engine e2(1210389477);
  
  default_random_engine e3;
  e3.seed(32767);
  default_random_engine e4(32767);

  for (size_t i=0; i!=100; ++i) {
    if (e1() == e2()) {
      cout << "different seed enginee match at iteration:" << i << endl;
    }
  
    if (e3() != e4()) {
      cout << "same seed enginee dismatch at iteration:" << i << endl;
    }
  }
  cout << endl << endl << endl;



  //random float
  uniform_real_distribution<double> real_u(0, 1);
  for (size_t i=0; i!=10; ++i) {
    cout << real_u(e) << " ";
  }
  cout << endl;

  //real_u.reset();  //same result no matter this line comment out or not. Don't know reset() effect.
  for (size_t i=0; i!=10; ++i) {
    cout << real_u(e) << " ";
  }
  cout << endl;

  cout << "real_u.max() is " << real_u.max() << endl;
  cout << "real_u.min() is " << real_u.min() << endl;
  cout << endl << endl << endl;


  //normal distribution
  normal_distribution<>  n(4, 1.5); //avarage 4. deviation 1.5.
  map<double, unsigned> distributionMap;
  double random, key;
  int deviationTimes;
  //example from book
  unsigned v;
  vector<unsigned> vals(9);  //example from book.
  for (size_t i=0; i!=200; ++i) {
    random = n(e);
    cout << random << " ";
    deviationTimes = int( (random-4) / 1.5);
    key = 4 + deviationTimes*1.5;
    ++distributionMap[key];
    //example from book
    v = lround(random);
    if (v < vals.size()) {
      ++vals[v];
    }
  }
  cout << endl;

  for (auto &e: distributionMap) {
    cout << e.first << string(e.second, '*') << endl;
  }

  cout << endl;
  for (size_t j=0; j!=vals.size(); ++j) {
    cout << j << ": " << string(vals[j], '*') << endl;
  }

  cout << endl << endl << endl;


  //bernouli_distribution return bool 1:1
  bernoulli_distribution b;
  string resp;
  do {
    bool first = b(e);
    cout << (first ? "we go frist" : "you go frist") << endl;
    cout << "run" << endl;
    cout << '.' << endl;
    cout << '.' << endl;
    cout << '.' << endl;
    cout << endl;
    cout << "play again? Enter 'yes' or 'no'" << endl;
  } 
  while (cin >> resp && (resp[0] == 'y' || resp[0] == 'Y'));
  cout << endl << endl << endl;

  return 0;

}
