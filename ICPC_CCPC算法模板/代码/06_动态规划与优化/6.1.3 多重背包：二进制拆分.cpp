#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// 将一件重量 w、价值 v 的物品加入 0/1 背包，原地更新 dp[0..W]，容量倒序。
// 要求 w>0；默认 dp 为容量不超过上限的最优值且允许不选，加法不得溢出；时间 O(W)。
// DP（Dynamic Programming）：动态规划；dp[j] 表示容量不超过 j 时可获得的最大价值。
void zero_one(vector<i64>& dp, int w, i64 v) {
    assert(w > 0);
    for (int c = (int)dp.size() - 1; c >= w; --c) {
        dp[c] = max(dp[c], dp[c - w] + v);
    }
}

// 依赖手册 6.1.1「0/1 背包」，以上已展开其实现。
// 将最多 count 件重量 w、价值 v 的物品加入多重背包，二进制拆分后更新 dp。
// 要求 w>0、count>=0，价值乘积及转移和不溢出；时间 O(W*log(count+1))。
// DP（Dynamic Programming）：动态规划；dp[j] 表示容量不超过 j 时可获得的最大价值。
void bounded(vector<i64>& dp, int w, i64 v, int count) {
    assert(w > 0 && count >= 0);
    for (i64 k = 1; count > 0; k *= 2) {
        int take = (int)min<i64>(k, count);
        count -= take;
        i64 weight = 1LL * w * take;
        if (weight < (int)dp.size()) {
            zero_one(dp, (int)weight, v * take);
        }
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
