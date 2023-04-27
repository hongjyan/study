#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    unsigned int num;
    cin >> num;
    std::vector<unsigned int> ui_vec;
    ui_vec.reserve(num);
    while (num--) {
        unsigned int temp;
        std::cin >> temp; 
        ui_vec.emplace_back(temp);
    }
    std::sort(ui_vec.begin(), ui_vec.end());
    auto end = std::unique(ui_vec.begin(), ui_vec.end());
    for (auto it=ui_vec.begin(); it!=end; ++it) {
        printf("%d\n", *it);
    }
}