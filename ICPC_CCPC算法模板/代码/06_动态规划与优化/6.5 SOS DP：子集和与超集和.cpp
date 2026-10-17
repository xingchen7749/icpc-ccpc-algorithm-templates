#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// 返回每个掩码 S 的子集和 Σ(T⊆S) f[T]；按值处理，不修改调用者的数组。
// 要求 0<=n<31、f.size()==2^n 且内存足够，累加不溢出；时间 O(n*2^n)。
// SOS DP（Sum Over Subsets Dynamic Programming）：子集和动态规划。
// f[mask] 初始为单个集合的值，逐位转移后为 mask 的所有子集的值之和。
vector<i64> subset_sum(vector<i64> f, int n) {
    assert(0 <= n && n < 31 && f.size() == (1ULL << n));
    for (int b = 0; b < n; ++b) {
        for (int s = 0; s < (1 << n); ++s) {
            if (s >> b & 1) {
                f[s] += f[s ^ (1 << b)];
            }
        }
    }
    return f;
}
// 返回每个掩码 S 的超集和 Σ(T⊇S) f[T]，掩码 0 表示空集。
// 要求 0<=n<31、f.size()==2^n 且内存足够，累加不溢出；时间 O(n*2^n)。
// f[mask] 初始为单个集合的值，逐位转移后为 mask 的所有超集的值之和。
vector<i64> superset_sum(vector<i64> f, int n) {
    assert(0 <= n && n < 31 && f.size() == (1ULL << n));
    for (int b = 0; b < n; ++b) {
        for (int s = 0; s < (1 << n); ++s) {
            if (!(s >> b & 1)) {
                f[s] += f[s ^ (1 << b)];
            }
        }
    }
    return f;
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
