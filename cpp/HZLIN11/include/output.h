#ifndef OUTPUT
#define OUTPUT
#include <iostream>
#include <vector>
#include <list>
#include "src/output.cpp"
#include "src/output_lst.cpp"
template <typename T>
void output(const T&);

template <typename T>
void output_lst(const std::list<T> &);
#endif
