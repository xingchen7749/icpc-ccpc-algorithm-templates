#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// 计算 Σ(i=1..n) floor(n/i)，按商相同的最大连续区间合并贡献。
// 要求 n>=0，n=0 返回 0；结果用 i128，时间 O(sqrt(n))，额外空间 O(1)。
i128 quotient_sum(i64 n) {
    assert(n >= 0);
    i128 ans = 0;
    for (i64 l = 1; l <= n;) {
        i64 q = n / l, r = n / q;
        ans += i128(r - l + 1) * q;
        if (r == n) break;
        l = r + 1;
    }
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
