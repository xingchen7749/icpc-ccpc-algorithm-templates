#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// 将可无限使用的重量 w、价值 v 的物品加入完全背包，原地更新 dp，容量正序。
// 要求 w>0，dp 使用容量 0..W；当前初始化约定为不要求恰好装满，时间 O(W)。
// DP（Dynamic Programming）：动态规划；dp[j] 表示容量不超过 j 时可获得的最大价值。
void complete(vector<i64>& dp, int w, i64 v) {
    assert(w > 0);
    for (int c = w; c < (int)dp.size(); ++c) {
        dp[c] = max(dp[c], dp[c - w] + v);
    }
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
