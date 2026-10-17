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
// 依赖 qpow：见 5.1「快速幂」（代码/05_数学/5.1 快速幂.cpp）。
// 要求 mod 是素数，且 a % mod != 0。
// 用费马小定理返回 a 在模 mod 下的逆元，结果在 [0,mod-1]。
// 要求 mod 是大于 1 的素数且 a 不被 mod 整除；本函数不检查素性。
// 支持负 a，依赖 qpow；时间 O(log mod)，空间 O(1)。
i64 inverse_fermat(i64 a, i64 mod) {
    assert(mod > 1 && a % mod != 0);
    return qpow(a, mod - 2, mod);
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
