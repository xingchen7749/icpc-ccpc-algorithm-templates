#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
// #define int long long

// 由 a[1..n] 构造前缀和 s，s[0]=0；[l,r] 的和为 s[r]-s[l-1]。
// 忽略 a[0]，不修改 a；要求累加不溢出 i64，时间和空间均为 O(n)。
vector<i64> prefix_sum(const vector<i64>& a) {
    vector<i64>s(a.size());
    for(int i=1;i<(int)a.size();++i)s[i]=s[i-1]+a[i];
    return s; // sum[l,r] = s[r]-s[l-1]
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
