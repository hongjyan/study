#include <cstdlib> 
#include <ctime> 
#include <iostream>
#include <random>

using namespace std;

int main() 
{ 
    srand((unsigned)time(0)); 
    int i;
    i = (rand()%6)+1; 
    cout << i << "\n"; 



    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(0,6); // distribution in range [1, 6]


    std::pair<int, int> pair = std::make_pair(dist6(rng), dist6(rng));
    std::cout << "[" << pair.first << "," << pair.second << "]" << std::endl;
}
