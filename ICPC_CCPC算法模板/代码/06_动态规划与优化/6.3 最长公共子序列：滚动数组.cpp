#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// 返回字符串 a、b 的最长公共子序列长度，允许不连续；空串对应答案 0。
// 滚动数组不保存恢复方案所需的信息；时间 O(|a|*|b|)，空间 O(|b|)。
// LCS（Longest Common Subsequence）：最长公共子序列。
// DP（Dynamic Programming）：动态规划。
int lcs(const string& a, const string& b) {
    // dp[j]：当前已处理的 a 前缀与 b 的前 j 个字符的 LCS 长度，原地滚动更新。
    vector<int> dp(b.size() + 1);
    for (char x : a) {
        int diag = 0;
        for (int j = 1; j <= (int)b.size(); ++j) {
            int old = dp[j];
            if (x == b[j - 1]) {
                dp[j] = diag + 1;
            }
            else {
                dp[j] = max(dp[j], dp[j - 1]);
            }
            diag = old;
        }
    }
    return dp.back();
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
