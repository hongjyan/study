#include <iostream>
#include <vector>
#include <cstdio>
#include <algorithm>
#include <map>
using namespace std;

struct ShoppingCart
{
    struct Item
    {
        int price;
        int priority;
        int primary;
    };

    struct PrimaryItem
    {
        vector<int> prices = {0, 0, 0};  // 每个主件最多带2个附件. 元素0，1， 2分别对应 主，附1， 附2.
        vector<int> value = {0, 0, 0};
    };

    void input()
    {
        cin >> capacity_ >> count_;

        items_.clear();
        Item temp;
        for (int i=0; i!=count_; ++i) {
            cin >> temp.price >> temp.priority >> temp.primary;
            items_.emplace_back(temp);
        }
    }

    void calculate()
    {
        // cout << "start calculate" << endl;
        vector<vector<int>> prices(100, vector<int>(3, 0));
        vector<vector<int>> values(100, vector<int>(3, 0));
        vector<vector<int>> indexes(100, vector<int>(3, 0));

        for (size_t i=0; i!=items_.size(); ++i) {
            auto a = items_[i].price / 10; // 除10以减少时间复杂度。后面计算出结果后在乘10.
            auto b = items_[i].priority;
            auto c = items_[i].primary;
            if (c == 0) {  // 主件
                prices[i+1][0] = a;
                values[i+1][0] = a * b;
                indexes[i+1][0] = i+1;
            } else { // 附件信息更新到主件中去
                if (prices[c][1] == 0) { 
                    prices[c][1] = a; values[c][1] = a*b; indexes[c][1] = i+1;
                } else {
                    prices[c][2] = a; values[c][2] = a*b; indexes[c][2] = i+1;
                }
            }
        }

        int n = capacity_ / 10;
        int m = count_;
        // dp[0][0]的值预留为1， 作为起始条件。
        vector<vector<int>> dp(n+1, vector<int>(m+1, 0));
        for (int i=1; i<=n; ++i) {
            for (int j=1; j<=m; ++j) {
                int a = prices[j][0], b = values[j][0];
                int c = prices[j][1], d = values[j][1];
                int e = prices[j][2], f = values[j][2];
                dp[i][j] = i >= a ? max(dp[i-a][j-1] + b, dp[i][j-1]) : dp[i][j-1];
                dp[i][j] = i >= (a+c) ? max(dp[i-a-c][j-1] + b + d, dp[i][j-1]) : dp[i][j-1];
                dp[i][j] = i >= (a+e) ? max(dp[i-a-e][j-1] + b + f, dp[i][j-1]) : dp[i][j-1];
                dp[i][j] = i >= (a+c+e) ? max(dp[i-a-c-e][j-1] + b + d + f, dp[i][j-1]) : dp[i][j-1];
            }
        }
        cout << dp[n][m] * 10 << endl;


        int i = n, j = m;
        while (i > 0 && j > 0) {
            auto gap = dp[i][j] - dp[i][j-1];
            if ( 0 != gap) {
                selectedItems_.push_back(indexes[j][0]);
                if (gap == values[j][1]) {
                    selectedItems_.push_back(indexes[j][1]);
                } else if (gap == values[j][2]) {
                    selectedItems_.push_back(indexes[j][2]);
                } else {
                    selectedItems_.push_back(indexes[j][1]);
                    selectedItems_.push_back(indexes[j][2]);
                }
                i -= gap;
                --j;
            } else {
                --j;
            }
        }
        std::sort(selectedItems_.begin(), selectedItems_.end());
        for (auto e : selectedItems_) {
            cout << e << " ";
        }
        cout << endl;
    }

private:
    int capacity_;
    int count_;
    vector<Item> items_;
    vector<int> selectedItems_;
};

int main() {
    ShoppingCart shoppingCart;
    shoppingCart.input();
    shoppingCart.calculate();
    return 0;
}