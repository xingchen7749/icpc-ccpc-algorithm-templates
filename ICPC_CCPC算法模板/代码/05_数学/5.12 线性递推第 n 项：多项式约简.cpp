#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// 已知 f[t]=c[0]f[t-1]+...+c[k-1]f[t-k]，返回 f[n]%mod。
// init[0..k-1] 为前 k 项，c 长度相同且 k>0；要求 mod>0，初值和系数已在 [0,mod)。
// 用多项式约简求第 n 项，时间 O(k²log(n+1))，空间 O(k)。
i64 linear_nth(const vector<i64>& init, const vector<i64>& c, u64 n, i64 mod) {
    int k = c.size();
    assert(k > 0 && init.size() == c.size() && mod > 0);
    // 内部合并：将次数小于 k 的多项式 a、b 相乘，再按递推特征关系约简并取模。
    // 返回长度 k 的系数数组，不修改输入；时间 O(k²)。
    auto combine = [&](const vector<i64>& a, const vector<i64>& b) {
        vector<i64> t(2 * k - 1);
        for (int i = 0; i < k; ++i) {
            for (int j = 0; j < k; ++j) {
                t[i + j] = (i128(t[i + j]) + i128(a[i]) * b[j]) % mod;
            }
        }
        for (int i = 2 * k - 2; i >= k; --i) {
            for (int j = 1; j <= k; ++j) {
                t[i - j] = (i128(t[i - j]) + i128(t[i]) * c[j - 1]) % mod;
            }
        }
        t.resize(k);
        return t;
    };
    // r：快速幂累积的余式系数；x：当前二进制幂对应的余式，数组下标表示次数。
    vector<i64> r(k), x(k);
    r[0] = 1 % mod;
    if (k == 1) {
        x[0] = c[0];
    }
    else {
        x[1] = 1 % mod;
    }
    for (; n; n >>= 1, x = combine(x, x)) {
        if (n & 1) {
            r = combine(r, x);
        }
    }
    i64 ans = 0;
    for (int i = 0; i < k; ++i) {
        ans = (i128(ans) + i128(r[i]) * init[i]) % mod;
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
