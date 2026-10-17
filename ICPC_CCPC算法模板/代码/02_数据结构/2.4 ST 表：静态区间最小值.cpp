#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// ST（Sparse Table）：稀疏表，用于静态区间查询。
// RMQ（Range Minimum Query）：区间最小值查询，本模板默认实现的操作。
class ST {
private:
    int n;
    // st[i][j]：从 i 开始、长度为 2^j 的区间最小值。
    vector<vector<int>> st;

public:
    // 由 a[1..n] 构造静态区间最小值表，忽略 a[0]，要求 n>=1。
    // 预处理时间和空间 O(n log n)；原数组修改后需重新构造。
    ST(const vector<int>& a) {
        assert(a.size() >= 2);
        n = (int)a.size() - 1;
        if (n <= 0) return;

        int K = log2((double)n) + 1;
        st.assign(n + 1, vector<int>(K));

        for (int i = 1; i <= n; ++i) st[i][0] = a[i];

        for (int j = 1; j < K; ++j) {
            for (int i = 1; i + (1LL << j) - 1 <= n; ++i) {
                st[i][j] = min(st[i][j - 1], st[i + (1LL << (j - 1))][j - 1]);
            }
        }
    }

    // 1 下标，查询非空闭区间 [l, r] 的最小值
    int query(int l, int r) const {
        assert(1 <= l && l <= r && r <= n);
        int j = log2((double)(r - l + 1));
        return min(st[l][j], st[r - (1LL << j) + 1][j]);
    }
};

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
