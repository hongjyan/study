#include <iostream>
#include <vector>
template <typename T>
void output(const T &rv) {
    std::cout << "---------------------------------" << std::endl;
    int i = 0;
    while (i != rv.size()) {
	std::cout << rv[i] << std::endl;
	i++;
    }
}
