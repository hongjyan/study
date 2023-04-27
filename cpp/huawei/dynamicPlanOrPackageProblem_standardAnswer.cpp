#include <iostream>
#include <vector>
#include <cstdio>

using namespace std;

vector<int> knapsack(int capacity, vector<int>& w, vector<int>& v) {
    int n = w.size();
    vector<vector<int>> dp(capacity + 1, vector<int>(n + 1, 0));
    vector<vector<int>> path(capacity + 1, vector<int>(n + 1, 0));

    // 动态规划
    for (int i = 1; i <= capacity; i++) {
        for (int j = 1; j <= n; j++) {
            if (i >= w[j - 1] && dp[i - w[j - 1]][j - 1] + v[j - 1] > dp[i][j - 1]) {
                dp[i][j] = dp[i - w[j - 1]][j - 1] + v[j - 1];
                path[i][j] = j;
                printf("pick, dp[%d][%d] is %d, path[%d][%d] is %d.\n", i, j, dp[i][j], i, j, path[i][j]);
            } else {
                dp[i][j] = dp[i][j - 1];
                path[i][j] = path[i][j - 1];
                printf("ignore, dp[%d][%d] is %d, path[%d][%d] is %d.\n", i, j, dp[i][j], i, j, path[i][j]);
            }
        }
        std::cout << endl << endl;
    }
    cout << "best solution: " << dp[capacity][n] << endl;

    // 从 path 数组中反推最优解的具体组合
    int i = capacity;
    int j = n;
    vector<int> selectedItems;
    while (i > 0 && j > 0) {
        if (path[i][j] == j) {
            selectedItems.push_back(j);
            i -= w[j - 1];
            j--;
        } else {
            j--;
        }
    }

    return selectedItems;
}

int main() {
    int capacity = 10;
    vector<int> w = {2, 3, 4, 5};
    vector<int> v = {3, 4, 5, 6};

    vector<int> selectedItems = knapsack(capacity, w, v);

    cout << "Selected items: ";
    for (int i = selectedItems.size() - 1; i >= 0; i--) {
        cout << selectedItems[i] << " ";
    }
    cout << endl;

    return 0;
}
