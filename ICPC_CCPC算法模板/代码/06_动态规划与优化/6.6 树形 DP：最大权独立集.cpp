#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// 返回连通无向树的最大权独立集权值和，允许不选任何点；全负权时返回 0。
// g 和 w 均使用 1..n，0 号占位，要求权值和不溢出；迭代遍历耗时、空间均为 O(n)。
// DP（Dynamic Programming）：动态规划；此处在树上按子树合并状态。
i64 tree_independent_set(const vector<vector<int>>& g, const vector<i64>& w) {
    int n = (int)g.size() - 1;
    if (!n) return 0;
    vector<int> father(n + 1, -1), ord{1};
    father[1] = 1;
    for (int i = 0; i < n; ++i) {
        for (int v : g[ord[i]]) {
            if (v != father[ord[i]]) {
                father[v] = ord[i];
                ord.push_back(v);
            }
        }
    }
    // dp[u][0]：不选 u 的子树最大权值；dp[u][1]：选 u 的子树最大权值。
    vector<array<i64, 2>> dp(n + 1);
    for (int i = n - 1; i >= 0; --i) {
        int u = ord[i];
        dp[u][1] = w[u];
        for (int v : g[u]) {
            if (v != father[u]) {
                dp[u][0] += max(dp[v][0], dp[v][1]);
                dp[u][1] += dp[v][0];
            }
        }
    }
    return max(dp[1][0], dp[1][1]);
}

void solve() {
    // Read one test case. Arrays and vertices use 1..n.
}
signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T = 1;
    cin >> T; // Remove for a single test case.
    while (T--) solve();
}
