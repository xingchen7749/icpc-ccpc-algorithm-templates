#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// 由 a[1..n] 构造 n+2 项的差分数组，忽略占位 a[0]；不修改 a。
// 输入至少含 a[0]，允许 n=0；末项用于区间更新的边界标记，时间和空间 O(n)。
vector<i64> difference(const vector<i64>& a) {
    assert(!a.empty());
    int n = (int)a.size() - 1;
    vector<i64> diff(n + 2);
    for (int i = 1; i <= n; ++i) {
        diff[i] = a[i] - (i == 1 ? 0 : a[i - 1]);
    }
    return diff;
}
// 在差分数组 diff 上记录闭区间 [l,r] 加 v，不直接还原原数组；l>r 时不操作。
// 有效更新要求 l>=1 且 r+1 在数组内，通常分配 n+2 项；时间 O(1)。
void range_add(vector<i64>& diff, int l, int r, i64 v) {
    if (l > r) return;
    diff[l] += v;
    diff[r + 1] -= v;
}
// 将含 n+2 项的 diff 前缀累加，返回 a[1..n]，a[0]=0；忽略末项边界标记。
// 不修改 diff，可在后续继续更新并再次还原；时间和空间 O(n)。
vector<i64> restore(const vector<i64>& diff) {
    assert(diff.size() >= 2);
    int n = (int)diff.size() - 2;
    vector<i64> a(n + 1);
    for (int i = 1; i <= n; ++i) {
        a[i] = a[i - 1] + diff[i];
    }
    return a;
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
