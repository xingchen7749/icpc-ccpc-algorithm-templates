#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

// 快速幂：计算 a 的 b 次方对 mod 取模，返回 [0,mod-1] 内的余数。
// a 可为负，b 为非负指数，要求 mod>0；b=0 返回 1%mod，mod=1 返回 0。
// 用 i128 进行乘法以避免中间溢出；时间 O(log(b+1))，额外空间 O(1)。
i64 qpow(i64 a, u64 b, i64 mod) {
    a %= mod;
    if (a < 0) a += mod;

    i64 res = 1 % mod;
    while (b) {
        if (b & 1) res = i128(res) * a % mod;
        a = i128(a) * a % mod;
        b >>= 1;
    }
    return res;
}

// 依赖手册 5.1「快速幂」，以上已展开其实现。
const int MOD = 998244353;
// 依赖 qpow：见 5.1「快速幂」（代码/05_数学/5.1 快速幂.cpp），复制本节时一并复制。
// 对系数数组 a 原地做 NTT；inv=false 为正变换，true 为逆变换并乘长度逆元。
// 要求长度是 1..2^23 内的二次幂，系数在 [0,998244353)；时间 O(n log n)。
// NTT（Number Theoretic Transform）：数论变换，在有限域中进行离散变换。
// inv（inverse）：是否执行逆变换，true 表示逆变换。
void ntt(vector<int>& a, bool inv) {
    int n = a.size();
    assert(n > 0 && (n & (n - 1)) == 0 && n <= (1 << 23));
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) {
            j ^= bit;
        }
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        int wlen = qpow(3, (MOD - 1) / len, MOD);
        if (inv) {
            wlen = qpow(wlen, MOD - 2, MOD);
        }
        for (int i = 0; i < n; i += len) {
            int w = 1;
            for (int j = 0; j < len / 2; ++j) {
                int u = a[i + j], v = 1LL * a[i + j + len / 2] * w % MOD;
                a[i + j] = u + v;
                if (a[i + j] >= MOD) a[i + j] -= MOD;
                a[i + j + len / 2] = u - v;
                if (a[i + j + len / 2] < 0) a[i + j + len / 2] += MOD;
                w = 1LL * w * wlen % MOD;
            }
        }
    }
    if (inv) {
        int ni = qpow(n, MOD - 2, MOD);
        for (int& x : a) {
            x = 1LL * x * ni % MOD;
        }
    }
}
// 返回多项式 a、b 在模 998244353 下的普通卷积，系数按次数从 0 编号。
// 输入系数需已在 [0,MOD)，任一为空返回空数组；补齐后的变换长度 L<=2^23。
// 返回长度 |a|+|b|-1，不修改调用者的数组；时间 O(L log L)，空间 O(L)。
vector<int> convolution(vector<int> a, vector<int> b) {
    if (a.empty() || b.empty()) return {};
    int need = a.size() + b.size() - 1, n = 1;
    while (n < need) n <<= 1;
    assert(n <= (1 << 23));
    a.resize(n);
    b.resize(n);
    ntt(a, false);
    ntt(b, false);
    for (int i = 0; i < n; ++i) {
        a[i] = 1LL * a[i] * b[i] % MOD;
    }
    ntt(a, true);
    a.resize(need);
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
