#include <set>
#include <iostream>
#include <algorithm>

// int main()
// {
//     using namespace std;
//     set<int> s1{1, 2, 7};
//     set<int> s2{2, 3, 4};
//     s1.merge(s2);
//     for_each(s1.begin(), s1.end(), [](const auto& e) { cout << e << endl;})

//     return 0;
// }

// print out a container
template <class Os, class K>
Os& operator<<(Os& os, const std::set<K>& v) {
    os << '[' << v.size() << "] {";
    bool o{};
    for (const auto& e : v)
        os << (o ? ", " : (o = 1, " ")) << e;
    return os << " }\n";
}
 
int main()
{
    std::set<char>
        p{ 'C', 'B', 'B', 'A' }, 
        q{ 'E', 'D', 'E', 'C' };
 
    std::cout << "p: " << p << "q: " << q;
 
    // p.merge(q);
 
    std::cout << "p.merge(q);\n" << "p: " << p << "q: " << q;
}