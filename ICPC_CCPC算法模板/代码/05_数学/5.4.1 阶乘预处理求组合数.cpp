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
// 依赖 qpow：见 5.1「快速幂」（代码/05_数学/5.1 快速幂.cpp），复制本节时一并复制。
struct Comb {
    i64 p;
    // fac[i]：i! 模 p；ifac[i]：i! 在模 p 下的乘法逆元。
    vector<i64> fac, ifac;
    // 预处理 0..n 的阶乘和逆阶乘，用于模素数 p 的组合数查询。
    // 要求 0<=n<p 且 p 是素数（不检查素性）；时间 O(n+log p)，空间 O(n)。
    Comb(int n, i64 p) : p(p), fac(n + 1, 1), ifac(n + 1, 1) {
        assert(n < p && p >= 2);
        for (int i = 1; i <= n; ++i) {
            fac[i] = i128(fac[i - 1]) * i % p;
        }
        ifac[n] = qpow(fac[n], p - 2, p);
        for (int i = n; i > 0; --i) {
            ifac[i - 1] = i128(ifac[i]) * i % p;
        }
    }
    // 返回 C(n,k)%p；k<0 或 k>n 返回 0，有效查询要求 0<=n<=预处理上限。
    // 使用阶乘和逆阶乘，单次时间 O(1)。
    i64 C(int n, int k) const {
        if (k < 0 || k > n) return 0;
        assert(n < (int)fac.size());
        return i128(fac[n]) * ifac[k] % p * ifac[n - k] % p;
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
