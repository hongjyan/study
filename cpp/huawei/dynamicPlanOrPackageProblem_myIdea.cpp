// 下面代码是错误的
#include <iostream>
#include <vector>
#include <cstdio>
using namespace std;

int max(int a, int b)
{
    return a > b ? a : b;
}

struct ShoppingCart
{
    struct Item
    {
        int price;
        int priority;
        int category;
    };

    void input()
    {
        cin >> capacity_ >> count_;

        items_.clear();
        Item temp;
        for (int i=0; i!=count_; ++i) {
            cin >> temp.price >> temp.priority >> temp.category;
            items_.emplace_back(temp);
        }
    }

    int bestSolution()
    {
        return bestSolution(capacity_, count_);
    }

    int bestSolution(int capacity, int count)
    {
        if (count == 0) return 0;

        int ret = 0;
        if (count == 1) {
            for (int i=0; i!=count; ++i) {
                if (items_[i].price <= capacity) ret = max(ret, itemValue(i));
            }
            return ret;
        }
        return max(bestSolution(capacity, count - 1)
            , bestSolution(capacity - items_[count-1].price, count-1) + itemValue(count-1));
    }

// private:
    int capacity_;
    int count_;
    vector<Item> items_;

    int itemValue(int i)
    {
        return items_[i].priority;
    }

};

int main() {
    ShoppingCart shoppingCart;
    shoppingCart.capacity_ = 10;
    shoppingCart.count_ = 4;
    shoppingCart.items_ = {{2, 3, 0}, {3, 4, 0}, {4, 5, 0}, {5, 6, 0}};
    // shoppingCart.input();
    // shoppingCart.calculate();
    cout << shoppingCart.bestSolution() << endl;
    return 0;
}
