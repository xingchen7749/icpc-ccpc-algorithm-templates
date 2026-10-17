#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// a[0] is unused; xs[0] is also unused.
// 将 a[1..n] 排序去重，返回带 0 号占位的有序值表 xs；不修改原数组。
// 要求 a 至少包含占位项；时间 O(n log n)，空间 O(n)。
template <class T>
vector<T> compress(const vector<T>& a) {
    vector<T> xs(a.begin() + 1, a.end());
    sort(xs.begin(), xs.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());
    xs.insert(xs.begin(), T{});
    return xs;
}
// 返回 x 在有序离散值表 xs[1..m] 中的 lower_bound 位置，作为 1 下标的秩。
// 通常要求 x 已在值表中；否则返回插入位置，可能为 m+1。时间 O(log m)。
template <class T>
int rank_of(const vector<T>& xs, T x) {
    return lower_bound(xs.begin() + 1, xs.end(), x) - xs.begin();
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
