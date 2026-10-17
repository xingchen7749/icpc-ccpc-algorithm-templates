#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
#define endl '\n'
// #define int long long

using u128 = __uint128_t;
// 返回 (a*b)%m，要求 m>0；使用 u128 保存乘积，支持完整 u64 范围，O(1)。
u64 mul(u64 a, u64 b, u64 m) { return u128(a) * b % m; }
// 完整 u64 模数专用快速幂；5.1「快速幂」的 mod 为 i64，不能覆盖这里的范围。
// 内部模快速幂：用于素性测试，返回 a^b%m；调用场景要求 m>1。
// 模数支持完整 u64 范围，乘法通过 mul 防溢出；时间 O(log(b+1))。
u64 pw(u64 a, u64 b, u64 m) {
    u64 r = 1;
    for (; b; b >>= 1, a = mul(a, a, m)) {
        if (b & 1) {
            r = mul(r, a, m);
        }
    }
    return r;
}
// 判断无符号 64 位整数 n 是否为素数，n<2 返回 false。
// 使用覆盖完整 u64 范围的确定性 Miller-Rabin 底数，固定轮数下耗时 O(log n) 次模乘。
bool is_prime(u64 n) {
    if (n < 2) return false;
    for (u64 p : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
        if (n % p == 0) return n == p;
    }
    u64 d = n - 1;
    int s = 0;
    while (!(d & 1)) {
        d >>= 1;
        ++s;
    }
    for (u64 a : {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL}) {
        if (a % n == 0) continue;
        u64 x = pw(a % n, d, n);
        if (x == 1 || x == n - 1) continue;
        bool ok = false;
        for (int r = 1; r < s; ++r) {
            x = mul(x, x, n);
            if (x == n - 1) {
                ok = true;
                break;
            }
        }
        if (!ok) return false;
    }
    return true;
}

// 依赖手册 5.6「Miller–Rabin：64 位素性测试」，以上已展开其实现。
// Pollard-Rho 随机寻找并返回 n 的一个非平凡因子，要求 n 是合数且 n>=4。
// 偶数直接返回 2；奇数失败时重新尝试，无确定的最坏运行时间保证。
u64 rho(u64 n) {
    if (n % 2 == 0) return 2;
    static mt19937_64 rng(712367821);
    while (true) {
        u64 c = rng() % (n - 1) + 1, x = rng() % n, y = x, d = 1;
        // 当前 Rho 轮的迭代函数：返回 (v²+c)%n，用 128 位中间量避免平方和加法溢出。
        auto f = [&](u64 v) { return (u64)((u128(mul(v, v, n)) + c) % n); };
        while (d == 1) {
            x = f(x);
            y = f(f(y));
            d = gcd(x > y ? x - y : y - x, n);
        }
        if (d != n) return d;
    }
}
// 把 n 的所有质因子（含重数）追加到 out，要求 n>=1；n=1 不添加任何元素。
// 不清空、不排序 out；先判断素数，再用 Rho 分治，运行时间具有随机性。
void factor(u64 n, vector<u64>& out) {
    assert(n >= 1);
    if (n == 1) return;
    if (is_prime(n)) {
        out.push_back(n);
        return;
    }
    u64 d = rho(n);
    factor(d, out);
    factor(n / d, out);
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
