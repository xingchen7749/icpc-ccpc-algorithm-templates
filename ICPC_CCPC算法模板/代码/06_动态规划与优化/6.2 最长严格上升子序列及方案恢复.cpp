#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// 返回 a[1..n] 的一条最长严格上升子序列对应的原数组下标列表，忽略 a[0]。
// 结果列表不带占位，不保证字典序最小；时间 O(n log n)，空间 O(n)。
// LIS（Longest Increasing Subsequence）：最长严格上升子序列。
vector<int> lis_indices(const vector<i64>& a) {
    // tail[k]：长度为 k+1 的严格上升子序列可取得的最小末尾值。
    vector<i64> tail;
    // at[k]：tail[k] 对应的原数组下标；prev[i]：恢复方案时 i 的前驱下标，-1 表示无前驱。
    vector<int> at, prev(a.size(), -1);
    for (int i = 1; i < (int)a.size(); ++i) {
        int k = lower_bound(tail.begin(), tail.end(), a[i]) - tail.begin();
        if (k) prev[i] = at[k - 1];
        if (k == (int)tail.size()) {
            tail.push_back(a[i]);
            at.push_back(i);
        }
        else {
            tail[k] = a[i];
            at[k] = i;
        }
    }
    vector<int> ans;
    if (at.empty()) return ans;
    for (int i = at.back(); i != -1; i = prev[i]) ans.push_back(i);
    reverse(ans.begin(), ans.end());
    return ans;
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
