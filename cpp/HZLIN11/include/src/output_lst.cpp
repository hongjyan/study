#include "../output.h"

template <typename T>
void output_lst(const std::list<T> &lst) {
    typedef typename std::list<T>::const_iterator it;
    it first = lst.begin();
    while (first != lst.end() ) {
	std::cout << *first << " ";
	++first;
    }
    std::cout << std::endl;
    std::cout << "-------------------------" << std::endl;
}
